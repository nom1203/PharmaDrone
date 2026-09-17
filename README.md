# PharmaDrone

## Collaborators

- Ajlaan
- Irshaad
- Hemkesh
- Saad
- Joyven

PharmaDrone is a small C-based prescription-ordering project that simulates finding medicines across multiple pharmacies, choosing the best supplier for each item, and dispatching a drone to collect and deliver the order.

The repository contains both:
- a terminal-based application for interactive prescription ordering
- a lightweight local web UI served by a CivetWeb HTTP server
- CSV-backed pharmacy data and module-level unit tests

---

## What the project does

The app lets a user:
- load a prescription from a text file or enter medication names manually
- search the pharmacy catalog for in-stock options by medicine name
- compare prices, stock, and ETA (simulated drone travel time)
- select a pharmacy for each item in the prescription
- confirm the order and simulate a drone delivery flow
- view a receipt with the total cost

The project is intentionally a simulation: it does not connect to a real database or a real drone, but it does model the decision-making and flow of a pharmacy ordering system.

---

## Repository structure

```text
PharmaDrone/
├── README.md
├── Assignment2026.pdf
├── pharmadrone.exe
├── pharmacdrone-server.exe
├── data/
│   ├── pharmacies.csv
│   └── sample_prescription.txt
├── include/
│   ├── common.h
│   ├── order.h
│   ├── pharmacy.h
│   ├── prescription.h
│   └── utils.h
├── src/
│   ├── main.c
│   ├── order.c
│   ├── pharmacy.c
│   ├── prescription.c
│   ├── server.c
│   └── utils.c
├── tests/
│   ├── test_order.c
│   └── test_pharmacy.c
├── vendor/
│   ├── civetweb.h
│   └── libcivetweb.a
└── web/
    ├── app.js
    ├── index.html
    └── style.css
```

---

## Core modules

### `include/common.h`
Defines shared constants and core data structures:
- `Pharmacy`
- `Medication`
- `OrderItem`
- maximum counts for pharmacies, medicines, and prescription items

### `include/pharmacy.h`
Provides the catalog API:
- initialize built-in pharmacy data
- load pharmacy records from CSV
- save the catalog back to CSV
- get pharmacy records by index
- search for medicines by name
- find matching pharmacies with stock available
- print the full catalog

### `include/prescription.h`
Handles prescription intake:
- file-based upload
- manual keyboard entry
- storing medicines into the order workflow

### `include/order.h`
Coordinates the order pipeline:
- full interactive order flow
- total price calculation
- drone dispatch simulation and receipt output

### `include/utils.h`
Housekeeping helpers used throughout the project:
- trim strings
- lowercase conversion
- timing for animation
- output dividers

### `src/server.c`
Runs a tiny HTTP server using CivetWeb and exposes the pharmacy catalog and medicine match API to the browser UI:
- `GET /api/pharmacies`
- `GET /api/match?med=...`

### `web/*.html/js/css`
A simple browser dashboard with:
- a pharmacy catalog tab
- a prescription selection flow
- one-click medicine match selection
- drone dispatch animation
- order receipt display

---

## Pharmacy dataset and sample data

The catalog is initialized in `src/pharmacy.c` and a CSV file is also included in `data/pharmacies.csv`.

The CSV fields are:

```text
pharmacy_name,address,eta_minutes,medicine_name,price,stock
```

The default catalog includes several sample pharmacies and medicines such as:
- GreenLeaf Pharmacy
- CarePlus Chemist
- MediWell Pharmacy
- HealthHub Pharmacy
- QuickCare Drugstore

Sample prescription input is provided in `data/sample_prescription.txt`:

```text
Paracetamol
Cetirizine
```

---

## How the console app works

Run the main application:

```bash
./pharmadrone
```

The menu includes:
1. Upload prescription & order drone delivery
2. Browse all pharmacies & stock
3. Exit

The workflow is:
1. load a prescription from a file or type medicines manually
2. search each medicine across the pharmacy catalog
3. list matching pharmacies with price, stock, and ETA
4. choose a pharmacy for each medicine to include in the order
5. confirm the order
6. simulate a drone visiting each pharmacy and collecting items
7. print a summary receipt with the total cost

---

## Browser app

The repository also includes a browser-based UI served locally.

Start the local web server:

```bash
gcc -Wall -Wextra -std=c11 -Iinclude -Ivendor -o pharmacdrone-server src/utils.c src/pharmacy.c src/server.c vendor/libcivetweb.a
./pharmacdrone-server
```

Then open:

```text
http://localhost:8080
```

This UI uses the same underlying pharmacy logic and lets you:
- browse the catalog
- select medicines and matching pharmacies
- trigger a visual drone dispatch animation
- view an order summary

---

## Building the C application

Compile the main CLI:

```bash
gcc -Wall -Wextra -std=c11 -Iinclude -o pharmadrone src/utils.c src/pharmacy.c src/prescription.c src/order.c src/main.c
```

On Windows, use an `.exe` output name if needed:

```bash
gcc -Wall -Wextra -std=c11 -Iinclude -o pharmadrone.exe src/utils.c src/pharmacy.c src/prescription.c src/order.c src/main.c
```

---

## Running the tests

The project includes small unit tests for the pharmacy and order modules.

### Pharmacy tests

```bash
gcc -Wall -Wextra -std=c11 -Iinclude -o test_pharmacy src/utils.c src/pharmacy.c tests/test_pharmacy.c
./test_pharmacy
```

### Order tests

```bash
gcc -Wall -Wextra -std=c11 -Iinclude -o test_order src/utils.c src/pharmacy.c src/prescription.c src/order.c tests/test_order.c
./test_order
```

These tests validate:
- pharmacy initialization and bounds checks
- case-insensitive medicine matching
- exclusion of out-of-stock pharmacies
- correct total price calculations

---

## Notes on the implementation

This is a learning project and a simulation, not a production pharmacy system. Some intentional limitations are:
- stock is stored in memory and CSV files, not a real database
- medicine matching is exact and case-insensitive, not fuzzy
- there is no dosage/quantity tracking beyond the stock count
- no real delivery logistics, authentication, or payment flow is implemented
- the drone dispatch is a visual simulation

---

## Summary

PharmaDrone combines:
- a pharmacy catalog
- prescription intake
- search and selection logic
- cost calculation
- a drone simulation
- a local browser frontend
- unit tests for the core business logic

It is a good example of a small systems-style C project with modular design, shared header files, data-driven catalog loading, and a simple web layer on top of the same C backend logic.
