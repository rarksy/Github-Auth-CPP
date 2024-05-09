# Github-Auth

A simple authentication system using Githubs raw file system and MD5 hashing to "secure" user data.
Made in ~1 Day

# Info
- HWID Authenticated via CPU Serial, First Drive Found On System & current user logged in, all strings reversed then hashed with md5
- Hash syntax is the same as the c# version so active users will work on both versions

# Setup
- Provide link to auth file (either to raw github file or file on a server) for the app to access hashes
- Hit '2' To enter register setup in program, this will generate a users auth hash
- paste the hash to your file
- wait for cache to expire and enjoy


# Note
- Application seems to be auto caching user file, so new users might not be authed for ~5 minutes after adding, just wait a bit and it'll work
- This is by no means supposed to be 100% secure, its just a simple example of an authentication system in a console application in C++
- A Less Efficient, less secure version written in C# is available at https://github.com/rarksy/Github-Auth-CSharp
