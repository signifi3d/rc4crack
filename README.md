This tool won't necessarily crack an rc4 cipher, but will slim down the possible results to help lead to cracking it. It tries either a brute force or dictionary attack against the cipher and outputs only the results that include printable characters.

Usage: rc4crack -c HEXCIPHER {-b [-s luns][-m customcharset][-l lower upper] | -d -w /path/to/wordlist}[-h]
		Look, basically you input your cipher after -c then choose if you want a brute force (-b), or
 		dictionary (-d) attack. With a brute force attack you can choose what charsets you want to
 		include such as lower (l), upper (u), numeric (n), or symbols (s), just include them as one
 		string, no spaces. Or you can even specify a custom charset (-m), then you provide the upper
 		and lower limits (-l). Default charset is lun, default limits are 1 4. For a dictionary attack
 		just be sure to include a wordlist (-w).
