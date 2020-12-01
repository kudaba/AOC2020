# What is this?
This repository functions as a (very) basic example of how to build an application on top of GameCore. It may evolve a bit over time, but it really is intended as a simple launching point.

# What's included
The template includes a standard setup for Game development
* Game project that's compiled in all configurations
* Tools project that's only compiled on non-retail edit platforms (currently only Windows)
* Exe hook to launch into either Game or Tool dependening on configuration/platform
* Additional platform specific projects (i.e. Package project for Android development)

# How to use
It should be easy enough to do a file search for 'Template' and 'TMP' to find the places that might need updating, but here's a short list:

* Sharpmake projects - update to reflect proper names
* generate_and_open scripts - update to open correctly generated files
* TMP_Main - Update gamecore configuration settings
* AndroidPackage\res\values - update project name
* AndroidPackage\src\com\Template\Stub.java - rename folder (file required to open packages in Android Studio)
* Template/Bin/Editor/*.* - Make sure to rebuild and Layouts

# GameExample
For simplicity the Template project includes and launches the GameExample projects from GameCore. Eventually these should be replaced by your own projects, but feel free to keep them around as references.

# Unit Tests
Unit Tests are added to all build flavors to make it easy to make changes to GameCore itself if the need arises.