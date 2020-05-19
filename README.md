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

* Make available the possibility of update settings for files and projects from Settings Window.
* Integration with GIT/GitHUB/Bitbucket/Any other CVS
* Integration with Scintilla
* Implement Rebuild and Clean.
* Add the possibility to add/remove/modify files to the project in run time. This means that the editor will check such files and will show a pop up with a message in case some files had been added/removed/modified into the project directory. (look for wxFileSystemWatcher)
* Add the possibility to save/reload the layout of the IDE and set the default option in the menu.
* Change paths to files and directories from strings to wxFileName
* When there is no change in the project, we shouldn't rewrite the project file because. When it does, the order of the files is different.
* Spanish accents are not supported by the controls of wxWidgets, we need to add support to them.
* When there is just one project open, whe should build it whether is open a file or not.

Tomorrow:
---------

- Add Find/Replace tools with windows.
- Configure the tool chain based on the configuration file.
- Ensure every file outside the project has an absolute path. When file is absolute, we should set up this configuration on the project file.
- Implement a Console Log to log the output of the assembler, linker, and compiler.
