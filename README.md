# PharmaDrone — Prescription Upload & Drone Delivery

**SIS 2075 — Software Engineering 1, Mini Project**
**Company name:** _[fill in your team's chosen name]_
**Team members:** _[Member A — pharmacy module] · [Member B — prescription module] · [Member C — order/drone module]_

A console application, written in C, that simulates:

1. **Uploading a prescription** (from a text file or typed in manually).
2. **Searching pharmacies** for each medicine, showing price, stock, and drone ETA.
3. **Choosing a pharmacy** per medicine.
4. **Simulated drone dispatch** — the drone visits each chosen pharmacy, "picks up"
   the item, then "delivers" everything, ending in an itemized receipt.

This is a simulation: the pharmacy "database" is an in-memory hardcoded list
(`src/pharmacy.c`), and the drone is a text animation. It's built so a real data
source or dispatch API could later be swapped in without touching the other modules.

---

## 1. Project structure

```
pharmadrone/
├── include/            Header files (.h) — one per module, the "contract" each
│   ├── common.h         module exposes to the rest of the program
│   ├── utils.h
│   ├── pharmacy.h
│   ├── prescription.h
│   └── order.h
├── src/                Implementation files (.c)
│   ├── utils.c          shared helpers (string trimming, timing)
│   ├── pharmacy.c        [Member A] catalog & stock search
│   ├── prescription.c    [Member B] prescription upload
│   ├── order.c           [Member C] pharmacy selection + drone dispatch
│   └── main.c            integration: top-level menu, wires modules together
├── tests/              Unit tests, one file per testable module
│   ├── test_pharmacy.c
│   └── test_order.c
├── data/
│   └── sample_prescription.txt
├── Makefile
├── .gitignore
└── README.md
```

### Why it's split this way

The assignment requires the project to be broken into modules, each with its own
`.h`/`.c` files, assigned to a different team member, and unit tested before
integration. The three interactive modules map to the three team members:

| Module         | Files                                | Responsibility                                              |
|----------------|---------------------------------------|---------------------------------------------------------------|
| `pharmacy`     | `pharmacy.h` / `pharmacy.c`          | Pharmacy data, stock search, catalog printing                 |
| `prescription` | `prescription.h` / `prescription.c`  | Getting the prescription into the program (file or manual)     |
| `order`        | `order.h` / `order.c`                | Selecting a pharmacy per medicine, drone dispatch, receipt      |

`common.h` and `utils.c`/`utils.h` are shared infrastructure (structs, constants,
string helpers) that every module depends on. `main.c` contains no business
logic — it only calls into the three modules, which is the "integration" step
described in the brief.

---

## 2. Building and running

Requires `gcc` and `make`.

```bash
make            # builds the ./pharmadrone executable
./pharmadrone   # run it
```

```bash
make test       # builds and runs all unit tests
```

```bash
make clean      # removes all compiled binaries and object files
```

### Testing modules independently before integration

`make test` builds two small standalone test executables:

- `tests/test_pharmacy` links only `utils.c` + `pharmacy.c` — it tests the
  pharmacy module completely on its own, with no dependency on prescription
  handling, ordering, or the interactive menu.
- `tests/test_order` links `utils.c` + `pharmacy.c` + `prescription.c` +
  `order.c` and tests `order_calculate_total()` — the one piece of order logic
  that's pure calculation rather than interactive I/O, so it can be tested
  without simulating keyboard input.

This mirrors the brief's description of unit testing each module before final
integration: run `make test` after every change to a module, then only run the
full interactive app (`make && ./pharmadrone`) once its tests pass.

---

## 3. Git & GitHub workflow

This section is both a **guide for the team** and the **basis for the Git/GitHub
section of the report** — copy the relevant parts across and add your own
screenshots/command history once you've actually done it.

### 3.1 Initial setup (one team member)

```bash
git init
git add .
git commit -m "Initial project scaffold"
# Create an empty repo on GitHub, then:
git remote add origin <your-repo-url>
git branch -M main
git push -u origin main
```

Add the other two members as collaborators on GitHub
(**Settings → Collaborators**), or invite them if using a GitHub Organization.

### 3.2 Everyone else: cloning

```bash
git clone <your-repo-url>
cd pharmadrone
```

### 3.3 One feature branch per module, per member

Rather than everyone committing straight to `main`, each member works on their
own module in its own branch:

```bash
# Member A
git checkout -b feature/pharmacy-module
# ... work on include/pharmacy.h and src/pharmacy.c ...
git add include/pharmacy.h src/pharmacy.c
git commit -m "Improve stock search to handle partial name matches"
git push -u origin feature/pharmacy-module

# Member B
git checkout -b feature/prescription-module
# ... work on prescription.h / prescription.c ...

# Member C
git checkout -b feature/order-module
# ... work on order.h / order.c ...
```

On GitHub, open a **Pull Request** from each feature branch into `main`, have
a teammate review it, then **Merge**. This is the collaboration model the
brief specifically asks you to demonstrate — screenshot the PR list and at
least one merged PR for the report.

### 3.4 Demonstrating a merge conflict (bonus marks)

A good, easy way to generate and resolve a real conflict for the demo:

```bash
# Two members both edit the same line of README.md on different branches,
# e.g. both update the "Team members" line at the top of this file.
git checkout -b demo/conflict-a
# edit README.md, commit, push
git checkout main
git checkout -b demo/conflict-b
# edit the SAME line differently, commit, push
git checkout main
git merge demo/conflict-a      # merges cleanly
git merge demo/conflict-b      # <-- conflicts here
```

Git will mark the conflicting section in the file with `<<<<<<<`, `=======`,
`>>>>>>>`. Manually edit it to the resolved version, then:

```bash
git add README.md
git commit -m "Resolve merge conflict in README team list"
```

Screenshot the conflict markers and the resolution for the report.

### 3.5 Undoing commits (bonus marks)

Two different tools for two different situations — useful to show you
understand when to use each:

```bash
git revert <commit-hash>   # adds a NEW commit that undoes an old one.
                           # Safe for commits already pushed/shared.

git reset --soft HEAD~1   # moves the branch pointer back, keeps your changes
                           # staged. Use only on commits nobody else has pulled.
```

### 3.6 Stashing (bonus marks)

If you need to switch branches with unfinished work:

```bash
git stash                 # parks your uncommitted changes
git checkout main
# ... do something else ...
git checkout feature/pharmacy-module
git stash pop              # brings your changes back
```

### 3.7 Tagging a release (bonus marks)

Mark a milestone (e.g. "first fully working integrated version") so it's easy
to find or roll back to later:

```bash
git tag -a v1.0 -m "First fully integrated, tested version"
git push origin v1.0
```

### 3.8 Visualising history for the demo

```bash
git log --oneline --graph --all
```

This prints a text graph of every branch and merge — screenshot it for the
report, and show it live during the demo to narrate how the three modules'
histories came together.

### 3.9 Cloning on a second machine (to demonstrate distributed work)

For the demo, it's worth actually showing this live if possible: have a
second team member `git clone` the repo fresh on their own laptop and run
`make && make test` to prove the project is fully portable and not dependent
on one person's machine setup.

### 3.10 Command reference table (fill in as you use them)

| Command | What it does | How we used it |
|---|---|---|
| `git init` | Creates a new local repository | Set up the project at the start |
| `git clone` | Copies a remote repo to a local machine | Each member got a local copy |
| `git add` | Stages changes for the next commit | Before every commit |
| `git commit -m` | Records staged changes with a message | After finishing a logical unit of work |
| `git push` | Uploads local commits to GitHub | Sharing work with the team |
| `git pull` | Downloads and merges remote changes | Before starting new work each day |
| `git branch` | Lists / creates branches | One branch per module |
| `git checkout -b` | Creates and switches to a new branch | Starting work on a module |
| `git merge` | Combines one branch's history into another | Merging finished modules into `main` |
| `git stash` | Temporarily shelves uncommitted changes | Switching branches mid-task |
| `git revert` | Undoes a commit by adding a new inverse commit | Safely undoing a shared mistake |
| `git reset` | Moves the branch pointer, optionally discarding changes | Cleaning up local-only commits |
| `git tag` | Marks a specific commit as a named milestone | Marking `v0.1-scaffold`, `v1.0` |
| `git log --graph` | Visualises commit/branch/merge history | Demo + report screenshots |
| `.gitignore` | Excludes files from version control | Kept build artifacts (`*.o`, binaries) out of the repo |

---

## 4. Extending it further

Ideas worth mentioning as "future work" in the report:

- Replace the hardcoded `pharmacy_init()` data with a CSV/text file read at
  startup, so pharmacy stock can be updated without recompiling.
- Add fuzzy matching to `pharmacy_find_matches()` so small typos still match.
- Track dosage/quantity rather than just medicine names.
- Enforce a payload-weight or controlled-substance restriction list before
  a drone dispatch is allowed.
