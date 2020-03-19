# cbrute V2 - high performace brute-force tool

### Use cbrute V2 for

- Generating combinations and permutations
- Creating wordlists
- Benchmarking your system
- High performance tool
- Fast file-writing
- Multi-core proccessing

### Features

- Character-pool adjustment
  - Use predefined character sets or create your own set
  - Exclude certain characters from sets
- Fixed length or range bases generation
  - Set a minimum- and maximum combination length
- Permutations
  - Generate permuations of a given string
- Append & prepend functionality
  - Append a user-defined base string to the combinations as they are being generated
  - Perpend a user-defined base string before the combinations as they are being generated
  - Combine prepending and appending (e.g. xxxTEMPLATExxx)

### Design choices

- **C++** for fast multicore-processing while still keeping the code modern and maintainable
- **No graphical user inferface:** Scheudlue tasks in you terminal or write scripts to run different operations

### Arguments

```bash
$ cbrutev2 [arguments]
```

| Console argument      | Function                                          |
| --------------------- | ------------------------------------------------- |
| -lmin <value>         | Set the minimum length for the combinations       |
| -lmax <value>         | Set the maximum length for the combinations       |
| -c <value>            | Character set                                     |
| -f <path>             | Path for output file with results                 |
| -perm <value>         | String to generate permutations of                |
| -ascii                | Fill printable ASCII character-set                |
| -alphabetic (<value>) | Alphabetic characters l: lowercase u: uppercase   |
| -digit                | Digits from 0-9                                   |
| -special              | Special characters                                |
| -x <value>            | Characters to exclude                             |
| -prepend <value>      | String to prepend before the combinations         |
| -append <value>       | String to append to the combinations              |
| **-h**                | **Help menu with all the arguments listed above** |

### Legal

Any actions and or activities related to the material contained within this program is solely your responsibility. The misuse of the information in this website can result in criminal charges brought against the persons in question. The author will not be held responsible in the event any criminal charges be brought against any individuals misusing cbrute to break the law.

**Your usage of cbrute constitutes your agreement to the following terms.**

1. All the information provided on this site are for **educational purposes only**. cbrute's developer is no not responsible for any misuse of the information.

2. cbrute V2 is a program related to **Computer Security** and not a tool that promotes hacking / cracking.

3. I reserve the right to modify the disclaimer at any time without notice.

### Authors

* **Julian Schnabel** - *Initial work* - [SchnJulian](https://github.com/SchnJulian)