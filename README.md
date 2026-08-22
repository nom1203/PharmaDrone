# Campus Food Delivery Drone

Campus Food Delivery Drone is a small, self-contained C command-line application that simulates ordering food from multiple campus restaurants and watching a delivery animation. It's intended as an educational/demo project to practice C programming and CLI interaction.

Why this project

- Simple, focused codebase (single-file C program) that demonstrates basic program structure, user input handling, and console output/animation.
- Great for beginners learning C or for quick demos in classrooms or workshops.

Features

- Choose from multiple campus restaurants
- Browse each restaurant's menu
- Add items to a cart and review cart contents
- Remove items from the cart before checkout
- Confirm an order and watch a simple delivery animation

Requirements

- A C compiler (tested with gcc)
- Unix-like environment (Linux, macOS) or WSL on Windows

Build

Compile the program with gcc:

```bash
gcc menu.c -o menu
```

If you prefer, add common flags for stricter checks:

```bash
gcc -std=c11 -Wall -Wextra -O2 menu.c -o menu
```

Run

Start the application from the terminal:

```bash
./menu
```

Follow the on-screen prompts to browse restaurants, add items to your cart, and place an order.

Example session

1. Run `./menu`.
2. Choose a restaurant by entering its number.
3. Enter menu item numbers to add them to your cart.
4. View or remove items from the cart.
5. Confirm the order to see the delivery animation.

Project structure

- `menu.c` — main C source file containing the CLI program
- `README.md` — this file

Contributing

Contributions are welcome. To contribute:

1. Fork the repository
2. Create a branch for your feature or fix (e.g., `feat/input-validation`)
3. Open a pull request with a clear description of the change and motivation

Ideas for improvements

- Add input validation and better error handling
- Split code into multiple files for readability and testing
- Add unit tests (e.g., using a tiny testing framework or harness)
- Add Windows-compatible build instructions (native or using MinGW)

Troubleshooting

- If compilation fails, ensure you have a working C toolchain and try compiling with `-Wall` to surface warnings.
- On Windows, run the program inside WSL or a compatible terminal.

License

This project is licensed under the MIT License — add a LICENSE file to this repository if you want an explicit license file.

Author

nom1203

Acknowledgements

This project was created for learning and demonstration purposes. Thanks for trying it out!