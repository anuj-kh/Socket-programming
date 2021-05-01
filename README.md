# Socket-programming
The problem statement can be found [here](./assignment-1.pdf)<br/><br/>
The C code connects to a squid proxy via sockets and then sends a GET request for the index page of any website. 
When the proxy responds with a webpage, this code stores that as a HTML file.<br/>
It also parses the returned HTML for the http://info.in2p3.fr/ website, and then finds an image and sends a GET request for that image as well. 
Which is also downloaded and stored as an image.
