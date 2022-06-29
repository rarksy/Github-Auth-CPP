# Github-Auth

A simple authentication system using Githubs raw file system and MD5 encryption to secure user data.
Made in ~1 Day

# Info
- HWID Authenticated via CPU Serial, First Drive Found On System & current user logged in, all strings reversed then hashed with md5
- Hash syntax is the same as the c# version so active users will work on both versions

# Setup
- Provide link to auth file (either to raw github file or file on a server) for the app to access hashes
- Add a user by making an md5 hash using a desired username (must be lower case username, you can use https://www.miraclesalad.com/webtools/md5.php, enable "Uppercase hash(es)" on website)
- once user is prompted to enter password, the password entered will be hashed aswell as hwInfo & username and copied to clipboard, replace the username hash with the hash copied to clipboard
- Re-Enter username:password on next launch


# Note
- Application seems to be auto caching user file, so new users might not be authed for ~5 minutes after adding, just wait a bit and it'll work
- This is by no means supposed to be 100% secure, its just a simple example of an authentication system in a console application in C++
- A Less Efficient, less secure version written in C# is available at https://github.com/rarksy/Github-Auth-CSharp
