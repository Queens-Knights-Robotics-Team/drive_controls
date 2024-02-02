# Drive Controls
## Getting Started
### 1. If you haven't already, authenticate with both GitHub and GitLab using your SSH key:
### *Generate SSH key*
- open your terminal and run the following command, making sure to replacing the example email with your GitHub email
```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
```
- you'll then be prompted with a series of questions... simply keep pressing *`Enter`*, leaving everything default
- once your SSH key is generated, run the following command and copy the output to your clipboard:
```bash
cat ~/.ssh/id_ed25519.pub
```
### *Add SSH key to GitHub and GitLab*
**For GitHub:**
- go to https://github.com/settings/keys and sign in if you aren't already signed in
- click `New SSH key` and give your new key an appropriate title
- paste the contents of your clipboard into the box labelled `Key`
- click `Add SSH key` to finish
<!-- -->
**For GitLab:**
- go to https://gitlab.com/-/profile/keys and sign in ***using your GitHub account***
- click `Add new key` and paste the contents of your clipboard into the box labelled `key`
- give your new key an appropriate title and click `Add key` to finish
### 2. Try cloning this repository with the following command:
```bash
git clone --recursive git@github.com:Queens-Knights-Robotics-Team/drive_controls.git
```
### 3. Run the setup script for your operating system:
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
each script will check for for and install all of the necessary tools with the correct versions so you can get started ASAP!
### 4. Try deploying code by doing the following:
Change your working directory to '`.../drive_controls/testing`' and run the command:
```bash
python3.12 deploy.py bc
```
Once executed, the python script will *build* all of the *controller* specific code as specified by the flags *b* and *c*<br/>
