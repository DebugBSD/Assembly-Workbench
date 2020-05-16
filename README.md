# Assembly-Workbench
IDE to program in Assembly Language

BUGS
----

* On Windows there is a bug when using wxExecute with environment variables. It doesn't recognise the ml64.exe from the PATH environment variable. So, right now it's hardcoded.
* We shouldn't ask for save changes when closing the application without any modification in the files.
* On the settings window we need to pass arrays of strings to paths instead of a single string in one line.
* We need more control over the environemt variables. We need to get/set them for the different kind of assemblers and options.
* Permit only characters and numbers in the name of projects and files.

TODO
----

* Make available the possibility of update settings for files and projects.
* Integration with GIT/GitHUB/Bitbucket/Any other CVS
* Integration with Scintilla
* Implement Rebuild and Clean.
* Add the possibility to add/remove/modify files to the project in run time. This means that the editor will check such files and will show a pop up with a message in case some files had been added/removed/modified into the project directory. (look for wxFileSystemWatcher)
* Add Find/Replace tools (wxFindReplaceDialog).
* Implement a remember functionality to the New Project window. (wxStandardPaths)
* Implement a Projects view to contain all information related to the projects in the tree control.
* Ensure every file into the project has a relative path and every file outside of the project has an absolute path.