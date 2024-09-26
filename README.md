# 🚀 PageRank Project: Ranking the Web, One Click at a Time!

This **PageRank** project brings the power of Google's original ranking algorithm to life, using C to analyze the importance of web pages in a network of hyperlinks. Whether you're interested in search engines, graph theory, or just love solving cool algorithmic challenges, this project is for you!

## 🌐 Project Overview

In this project, you'll find two powerful methods to calculate the **PageRank** of websites in a graph:

- **Random Surfer Model**: Simulates the journey of a fictional web surfer clicking random links, with the chance to "get bored" and jump to a random site.
- **Markov Chain Model**: A mathematical approach that uses matrix multiplication to rigorously calculate the rank of each page based on its connections.

Both methods give insight into which pages are most important, much like the early days of Google!

---

## 📂 How to Build

To compile the project, simply run:

```bash
make
```

This will create the `pagerank` executable in the root directory, ready for action! 

---

## 🛠️ How to Run

To calculate the PageRank, run the following command:

```bash
./pagerank [OPTIONS]... [filename]
```

- **[filename]**: The path to the graph file in `.dot` format (representing web pages and hyperlinks).

### 💡 Example Usage:

```bash
./pagerank -r 120 -p 12 ./tests/graphs/unigraph.dot
```

In this example:
- The **random surfer model** runs for 120 steps with a boredom probability of 12%.
- The input graph (`unigraph.dot`) is a sample file located in the `tests/graphs/` folder, representing university-related websites.

Once you run this, you'll see the PageRank results printed in the terminal, like this:

```
CMS         0.0502000920
dCMS        0.2274320656
dGit        0.1778830020
guide       0.1982474347
forum       0.2573738626
leaderboard 0.0888635431
```

### 🔧 Command-Line Options:

- `-h`: Display a help message with usage instructions.
- `-r N`: Run the random surfer model for **N steps**.
- `-m N`: Perform **N iterations** of the Markov Chain to compute the PageRank.
- `-s`: Display graph statistics, including the number of nodes, edges, and in/out-degrees.
- `-p P`: Set the probability `p` for the random surfer model (default: 10%).

### ⚡ Quick Start:

If you're looking for a fast start, just run:

```bash
./pagerank -h
```

This will show you all available options and get you up to speed quickly.

---

## 📊 Input Graph Format

We use graphs in **DOT** format to represent the websites and their links. Here’s an example of what a graph looks like:

```
digraph WebGraph {
    SiteA -> SiteB;
    SiteB -> SiteC;
    SiteC -> SiteA;
}
```

- **Nodes** represent websites.
- **Edges** (`->`) represent hyperlinks from one site to another.

Feel free to contribute your own graphs in this format and test out the ranking!

---

## 🎉 Contributions Welcome!

I built this project as a way to dive deep into graph theory and ranking algorithms, but it's just the beginning. I’d love to see what you can add! Feel free to:
- Submit improvements or suggestions.
- Add new graph examples (just stick to the `.dot` format).
- Experiment with different probability values or larger graphs.

Together, we can make this project even more awesome!

---

## 🌱 What I Learned

Through this project, I gained a deeper understanding of:
- **Graph theory** and its practical applications in web ranking.
- Implementing the **Random Surfer** and **Markov Chain** models in C.
- Working with **dynamic memory** and managing complex data structures.
- Handling **command-line arguments** in C to create flexible and user-friendly programs.

It’s been an amazing journey, and I hope this project inspires others to explore algorithms and graph theory as well!

---

Let's make the web a more **rankable** place—one node at a time! 🌍💻

---
