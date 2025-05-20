<h1 align='center'>Welcome</h1>

<div align="center">
<img src="https://telegra.ph/file/9a2ef1310014cea3be04e.jpg" alt="ELAINA MD" width="300" />
<p align="center">
 <img src="https://komarev.com/ghpvc/?username=darakubalika&color=blue&label=Views" />
 </p>
<p align="center">
Hi.

v1.0.0
<br/>
</p>
</div>


## Setup fbtagui
Needed
-   [x] Only supports Ubuntu and Debian
-   [x] Minimum Ubuntu20.04 and Debian10 versions below are not recommended
-   [x] Requires root access
-   [x] Requires stable internet access

1. Download file setup fbtagui<br />

```bash
wget https://raw.githubusercontent.com/darakubalika/fbtagui/master/setup
```
or
```bash
curl -O https://raw.githubusercontent.com/darakubalika/fbtagui/master/setup
```
2. Give execute permission to the file<br />
```bash
chmod +x setup
```
3. run
```bash
./setup
```
SHA256sum
```bash
2f6e18d53a398e18e5961ed546ed1469fd3b9b40a368e19b361f4dd994e6843a
```
## Important
When the setup process is taking place, it is not recommended to stop the setup process forcibly because it can cause errors in your system. The solution if you experience being stuck in the checking or loading process for several hours even though your connection is secure, you can reboot your system and do the setup process again.

## Install browser
Here I give an example using the Chrome browser
1. Download Chrome using this official link. If the link is not working, you can check it directly on the [Google Chrome](https://google.com/chrome/?platform=linux) website
```bash
wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
```
or
```bash
curl -O https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
```
2. Install chrome
```bash
apt install ./google-chrome-stable_current_amd64.deb
```
3. Add chrome in the panel
4. Edit the Google Chrome launcher so that it can be used.
```bash
vi /usr/share/applications/google-chrome.desktop
```
or
```bash
nano /usr/share/applications/google-chrome.desktop
```
find the ```Exec=/usr/bin/google-chrome-stable %U``` section and add ```--no-sandbox``` like this ```Exec=/usr/bin/google-chrome-stable %U --no-sandbox```, If you have, you can save it and then Chrome can be run from the launcher.
## Command
```bash
Usage: fbta <command>
available commands:
 -h - for help
 start - start the program
 stop - stop the program
 restart - restart the program
 setpasswd - set your password
 --setwalpp - set your walpaper
Example:
"fbta start"
"fbta setpasswd"
"fbta --setwalpp image.jpg"
```

## Screenshots

<p align="center">
  <img src="images/desktop.png" width="80%" height="80%">
</p>

<p align="center">
  <img src="images/browser.png" width="80%" height="80%">
</p>
