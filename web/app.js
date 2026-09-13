const API = '';   // same origin -> no prefix

const state = {
  medicines: [],      // [{ name, matches:[], chosen:null }]
};

/* ---------- helpers ---------- */
const $  = (s, r=document) => r.querySelector(s);
const $$ = (s, r=document) => [...r.querySelectorAll(s)];
const esc = s => String(s).replace(/[&<>"']/g,
  c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));

async function api(path) {
  const r = await fetch(API + path);
  if (!r.ok) throw new Error(await r.text());
  return r.json();
}

/* ---------- tabs ---------- */
$$('.tab').forEach(t => t.onclick = () => {
  $$('.tab').forEach(x => x.classList.toggle('active', x === t));
  $$('.page').forEach(p =>
    p.classList.toggle('active', p.id === t.dataset.tab));
  if (t.dataset.tab === 'catalog') loadCatalog();
});

/* ---------- catalog tab ---------- */
async function loadCatalog() {
  const { pharmacies } = await api('/api/pharmacies');
  $('#catalogList').innerHTML = pharmacies.map(p => `
    <div class="card">
      <div class="title">${esc(p.name)}</div>
      <div class="sub">${esc(p.address)} · ETA ${p.eta.toFixed(1)} min</div>
      <div style="margin-top:8px">
        ${p.meds.map(m => `
          <div class="med-row">
            <span>${esc(m.name)}</span>
            <span class="price">$${m.price.toFixed(2)}</span>
            <span class="stock ${m.stock===0?'out':(m.stock<20?'low':'')}">
              stock ${m.stock}
            </span>
            <span></span>
          </div>`).join('')}
      </div>
    </div>`).join('');
}

/* ---------- order tab ---------- */
function parseLines(txt) {
  return txt.split(/\r?\n/).map(s => s.trim()).filter(Boolean);
}

$('#fileInput').onchange = async e => {
  const f = e.target.files[0];
  if (!f) return;
  $('#manual').value = await f.text();
};

$('#addMeds').onclick = async () => {
  const names = parseLines($('#manual').value);
  if (!names.length) return;

  state.medicines = [];
  for (const name of names) {
    const { matches } = await api(`/api/match?med=${encodeURIComponent(name)}`);
    state.medicines.push({ name, matches, chosen: null });
  }
  renderSelections();
};

function renderSelections() {
  const root = $('#selectionList');
  root.innerHTML = state.medicines.map((m, mi) => {
    if (!m.matches.length) {
      return `<div class="card">
        <div class="title">${esc(m.name)}</div>
        <div class="sub">No pharmacy currently has this in stock.</div>
      </div>`;
    }
    return `<div class="card">
      <div class="title">${esc(m.name)}</div>
      <div class="sub">Pick a pharmacy:</div>
      <div style="margin-top:8px">
        ${m.matches.map((ph, i) => `
          <div class="med-row ${m.chosen===i?'selected':''}"
               data-mi="${mi}" data-i="${i}">
            <span>${esc(ph.name)}<br><span class="sub">${esc(ph.address)}</span></span>
            <span class="sub">ETA ${ph.eta.toFixed(1)}m</span>
            <span class="price">$${ph.price.toFixed(2)}</span>
            <span class="stock">stock ${ph.stock}</span>
          </div>`).join('')}
      </div>
    </div>`;
  }).join('');

  $$('.med-row[data-mi]').forEach(row => row.onclick = () => {
    const mi = +row.dataset.mi;
    const i  = +row.dataset.i;
    state.medicines[mi].chosen = (state.medicines[mi].chosen === i) ? null : i;
    renderSelections();
    updateDispatch();
  });

  updateDispatch();
}

function selectedItems() {
  return state.medicines
    .filter(m => m.chosen !== null)
    .map(m => {
      const ph = m.matches[m.chosen];
      return { medicine: ph.med, pharmacy: ph.name, address: ph.address,
               price: ph.price, eta: ph.eta };
    });
}

function updateDispatch() {
  const items = selectedItems();
  $('#dispatch').disabled = items.length === 0;
  $('#dispatch').textContent =
    items.length
      ? `Dispatch drone for ${items.length} item(s) — $${total().toFixed(2)}`
      : 'Dispatch drone';
}

function total() {
  return selectedItems().reduce((s, i) => s + i.price, 0);
}

/* ---------- drone animation ---------- */
async function flyTo(name, address) {
  const el = $('#droneArea');
  el.textContent += `\n[Drone] Flying to ${name} (${address})…\n`;
  let bar = '';
  for (let i = 0; i <= 10; i++) {
    bar = `  [${'#'.repeat(i)}${' '.repeat(10 - i)}] ${i * 10}%`;
    el.lastChild.textContent = bar;
    await new Promise(r => setTimeout(r, 120));
  }
  el.textContent += `[Drone] Arrived at ${name}.\n`;
}

$('#dispatch').onclick = async () => {
  const items = selectedItems();
  if (!items.length) return;

  $('#receipt').classList.remove('show');
  $('#droneArea').textContent = 'Order confirmed. Preparing drone…';
  await new Promise(r => setTimeout(r, 600));

  // One flight per unique pharmacy
  const visited = new Set();
  for (const it of items) {
    if (visited.has(it.pharmacy)) continue;
    visited.add(it.pharmacy);
    await flyTo(it.pharmacy, it.address);

    for (const i2 of items.filter(x => x.pharmacy === it.pharmacy)) {
      $('#droneArea').textContent += `  -> Picking up ${i2.medicine} ($${i2.price.toFixed(2)})\n`;
    }
  }

  $('#droneArea').textContent +=
    '\n[Drone] All items collected. Heading to delivery address…\n';
  await flyTo('delivery address', '');
  $('#droneArea').textContent += '\n[Drone] Package delivered successfully!\n';

  // Receipt
  const r = $('#receipt');
  r.innerHTML = `
    <h3 style="margin:0 0 8px">Order receipt</h3>
    <table>
      ${items.map(i => `
        <tr>
          <td>${esc(i.medicine)}</td>
          <td>${esc(i.pharmacy)}</td>
          <td style="text-align:right">$${i.price.toFixed(2)}</td>
        </tr>`).join('')}
      <tr>
        <td colspan="2">TOTAL</td>
        <td style="text-align:right">$${total().toFixed(2)}</td>
      </tr>
    </table>`;
  r.classList.add('show');
};