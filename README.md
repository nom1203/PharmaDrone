# PharmaDrone

SIS 2075 – Software Engineering 1 Mini Project

Company: PharmaDrone

Team: Ajlaan (pharmacy module), Irshaad (prescription module), Hemkesh (order/drone module), Saad (testing/handling merge conflicts)

Team Leader: Joyven (coordinating/main module/testing)

## What this is

A C console app for ordering prescription medicine and having it delivered by drone.
You "upload" a prescription (either a text file or by typing the medicine names in),
the program checks which pharmacies have each medicine in stock and at what price,
you pick which pharmacy to buy from, and then it runs through a drone pickup and
delivery sequence and prints a receipt at the end.

The pharmacy stock is just a hardcoded list inside `pharmacy.c` for now — there's no
real database or actual drone involved, it's all simulated for the purposes of this
project.

## Project layout

```
pharmadrone/
├── include/
│   ├── common.h         shared structs (Pharmacy, Medication, OrderItem) and constants
│   ├── utils.h
│   ├── pharmacy.h
│   ├── prescription.h
│   └── order.h
├── src/
│   ├── utils.c          string helpers, sleep timing for the drone animation
│   ├── pharmacy.c       Ajlaan – pharmacy list, searching for medicine, stock levels
│   ├── prescription.c   Irshaad – reading in the prescription (file or manual)
│   ├── order.c          Hemkesh – picking a pharmacy per medicine, drone dispatch, receipt
│   └── main.c           menu that calls into the three modules above
├── tests/
│   ├── test_pharmacy.c
│   └── test_order.c
├── data/
│   └── sample_prescription.txt
└── .gitignore
```

We split it up this way so each of us could work on our part separately —
Ajlaan handles the pharmacy data and search, Irshaad handles getting the
prescription into the program, Hemkesh handles the ordering and drone part.
`main.c` doesn't really have any logic in it, it just ties the three pieces
together once they're all working.

## Building and running

No build system, just compile everything with one gcc command:

```bash
gcc -Wall -Wextra -std=c11 -Iinclude -o pharmadrone src/utils.c src/pharmacy.c src/prescription.c src/order.c src/main.c
```

Then run it:

```bash
./pharmadrone
```

(On Windows the output file will need a `.exe` on the end, e.g. `-o pharmadrone.exe`.)

### Running the tests

Each module has its own small test file so we could check it worked before
plugging it into the rest of the program. These build as separate mini-programs,
not part of the main app:

```bash
gcc -Wall -Wextra -std=c11 -Iinclude -o test_pharmacy src/utils.c src/pharmacy.c tests/test_pharmacy.c
./test_pharmacy

gcc -Wall -Wextra -std=c11 -Iinclude -o test_order src/utils.c src/pharmacy.c src/prescription.c src/order.c tests/test_order.c
./test_order
```

`test_pharmacy` only needs `utils.c` and `pharmacy.c` — it doesn't touch
prescriptions or ordering at all, so we can test the pharmacy search logic
completely on its own. `test_order` checks the total-cost calculation from
the order module.

## How it actually works

Run `./pharmadrone` and you get a menu:

1. Upload prescription & order drone delivery
2. Browse all pharmacies & stock
3. Exit

Option 1 asks how you want to give it your prescription — either point it at a
text file (one medicine name per line, see `data/sample_prescription.txt`) or
type the names in one by one. For each medicine it then lists every pharmacy
that currently has it in stock along with the price and how far away the drone
would need to fly. You pick a pharmacy (or skip that medicine), and once you've
gone through the whole list it shows the total cost and asks you to confirm.
Say yes and it plays out the drone flying to each pharmacy, picking things up,
then delivering everything and printing a receipt.

Option 2 just prints the full pharmacy list including anything that's out of
stock, in case you want to see what's available before uploading anything.

## Git workflow

We used a shared GitHub repo with one branch per person/module rather than
everyone pushing straight to `main`:

```bash
git clone <repo-url>
git checkout -b feature/pharmacy-module      # or prescription-module / order-module
# ... work, commit ...
git push -u origin feature/pharmacy-module
```

Each branch got merged into `main` through a Pull Request on GitHub once the
module's tests passed, rather than merging locally.

Commands we used, and why:

| Command | What it does | Where we used it |
|---|---|---|
| `git init` / `git clone` | start a repo locally / copy an existing one | setting up the repo, each of us cloning it |
| `git add`, `git commit -m` | stage and save changes | after finishing a piece of a module |
| `git push`, `git pull` | sync with GitHub | sharing work, getting teammates' changes |
| `git branch`, `git checkout -b` | create/switch branches | one branch per module |
| `git merge` | combine branches | merging a finished module into `main` |
| `git stash` | temporarily set aside uncommitted changes | switching branches mid-task without committing half-done work |
| `git revert` | undo a commit by adding a new commit that reverses it | fixing a mistake that had already been pushed |
| `git reset` | move the branch pointer back | cleaning up commits that hadn't been pushed yet |
| `git tag` | mark a specific commit | tagging the first fully working version |
| `git log --graph` | see branch/merge history as a diagram | checking how everything came together |
| `.gitignore` | stop certain files from being tracked | kept compiled binaries and `.o` files out of the repo |

We also deliberately caused and resolved a merge conflict (two of us editing
the same line of this README on different branches at the same time) to
practice resolving conflicts before it happened for real.

## Known limitations / things we'd add given more time

- Pharmacy stock is hardcoded rather than read from a file or database
- No fuzzy matching, so a typo in a medicine name won't match anything
- Doesn't track dosage or quantity, just whether a medicine is in stock
- No check for controlled substances or drone weight limits
