# Assembly-Workbench
IDE to program in Assembly Language

BUGS
----

* On Windows there is a bug when using wxExecute with environment variables. It doesn't recognise the ml64.exe from the PATH environment variable. So, right now it's hardcoded.
* We shouldn't ask for save changes when closing the application without any modification in the files.
* On the settings window we need to pass arrays of strings to paths instead of a single string in one line.
* We need more control over the environment variables. We need to get/set them for the different kind of assemblers and options.

TODO
----

* Make available the possibility of update settings for files and projects.
* Integration with GIT/GitHUB/Bitbucket/Any other CVS
* Integration with Scintilla
* Implement Rebuild and Clean.
* Add the possibility to add/remove/modify files to the project in run time. This means that the editor will check such files and will show a pop up with a message in case some files had been added/removed/modified into the project directory. (look for wxFileSystemWatcher)
* Add Find/Replace tools with windows.
* Add the possibility to save/reload the layout of the IDE and set the default option in the menu.
* Implement a Console Log to log the output of the assembler, linker, and compiler.
* Think about using buffers for files instead of the wxTextCtrl or any other class. Look at the wxWidgets Documentation. (See wxTestFile, it seems ideal!)

Tomorrow:
---------

- Permit only characters, numbers, spaces and some special characters in the name of projects and files.
- Implement a basic project file when creating a new empty project.
- Open Projects from menu.
- Open Files from the Projects Tree Window.
- Ensure every file into the project has a relative path and every file outside of the project has an absolute path.

