# Drive Controls
## Getting Started
### before you can clone this repository, you must authenticate with GitLab using your SSH key:
- open your terminal and run the following command:
```bash
cat ~/.ssh/id_ed25519.pub
```
- copy the output of the command to your clipboard
- go to https://gitlab.com/-/profile/keys and login with your GitHub account
- click `Add new key` and paste the contents of your clipboard into the box labelled `key`
- finally, title the key something appropriate and click `Add key` to finish
### try cloning this repository with the following command:
```bash
git clone --recursive git@github.com:Queens-Knights-Robotics-Team/drive_controls.git
```
### set up your development environment by doing the following:
Change your working directory to `.../drive_controls` and run the setup script that corresponds to your operating system
```bash
# for macOS
./setup-mac.zsh
```
```bash
# for Debian/Ubuntu Linux
./setup-deb-linux.sh
```
*if you are on Windows, you must use **Windows Subsystem for Linux** with **Ubuntu 20.04 LTS** or later (recommended)*<br/><br/>
each script checks for and installs all of the required tools for development so you don't have to!
### try deploying code by doing the following:
Change your working directory to '`.../drive_controls/testing`' and run the command:<br/>
```bash
python3 deploy.py --help
```
Once executed, the python script will print its usage to the terminal, allowing you to begin testing code for either a controller or keyboard & mouse<br/>
