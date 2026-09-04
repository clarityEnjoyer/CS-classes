# Functional Language Evaluator

A straightforward Haskell interpreter for a simplified functional programming language. It reads a file containing function definitions and data constructors, then evaluates the `main` expression step-by-step. The core engine uses pattern matching, lazy evaluation, and a Zipper data structure to navigate and reduce the abstract syntax tree.

## Key Features

*   **Pattern Matching:** Handles multi-clause functions and checks constructor arity on the fly.
*   **Zipper Navigation:** Pinpoints and reduces specific nodes in the AST without losing track of the overall expression context.
*   **Lazy Evaluation:** Only forces the evaluation of arguments when absolutely necessary to resolve a pattern.
*   **Custom Monadic Engine:** Uses a tailored `EvalM` monad to manage the environment (Reader), execution history (Writer), and an infinite-loop safeguard (State).
*   **Visual Trace:** Outputs a clean, step-by-step history of the entire reduction process, highlighting the evaluated nodes.

## Requirements

*   GHC (tested on versions 9.0.2 – 9.8.2)
*   Cabal 3.4 or newer

## Usage

You can build and run the project using Cabal from the root directory:

1.  Build the project:
    ```bash
    cabal build
    ```

2.  Run the evaluator on your source file:
    ```bash
    cabal run zadanie3 -- your_file.uhs
    ```
    *(Note: To prevent infinite loops like evaluating $\Omega$, the interpreter has a hardcoded limit of 30 reduction steps. If it hits the limit, it stops and prints the history up to that point).*
