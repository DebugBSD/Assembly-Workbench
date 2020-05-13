# Assembly-Workbench
IDE to program in Assembly Language

BUGS
----

* On Windows there is a bug when using wxExecute with environment variables. It doesn't recognise the ml64.exe from the PATH environment variable. So, right now it's hardcoded.
* We shouldn't ask for save changes when closing the application without any modification in the files.
* On the settings window we need pass arrays of strings to paths instead of a single string in one line.
* We need more control over the environemt variables. We need to get/set them for the different kind of assemblers and options.

TODO
----

* Pass linker to the settings. Right now everything related to the linker is hardcoded into MLINKER.CPP/H
* Make available the possibility of update settings for files and projects.
* Integration with GIT/GitHUB/Any other CVS
* Integration with Scintilla
