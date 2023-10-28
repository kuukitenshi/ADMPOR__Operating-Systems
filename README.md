# ADMPOR

This project was a collaborative effort involving two more students for our ```Operating Systems``` subject.

The objective of the project was to create a program that simulates the administrative system of a port, allowing various operations.

That was implemented in ```C language```, handling all the client's required operations and also employing ```synchronization mechanisms``` such as mutexes and semaphores for ```file read and write operations```. Furthermore, it provided us an opportunity to learn about ```memory management and analysis```, improving our debugging skills and proficiency with tools like ```Valgrind```.

All the functionalities were successfully implemented, and we received a ```perfect score```.

---
## Compilation

To compile the project and generate the executable, use the following command:
```bash
$ make
```

To create the required folders ```bin``` and ```obj``` for the correct compilation of the project, execute the command:
```bash
$ make folders
```
To clean all the ```object``` and ```executable``` files use:
```bash
$ make clean
```

---
## Running

This project can be runned using:
```bash
$ ./<executable> <configuration file>
```

Where the ```configuration file``` needs to have the following structure:
- **line 1**: max number of operations
- **line 2**: buffers length
- **line 3**: number of clients
- **line 4**: number of intermediaries
- **line 5**: number of enterprises
- **line 6**: file log name's
- **line 7**: file stats name's
- **line 8**: timer for the alamr