# Information about this project

I have created a multi-threaded Web Server using the C language and a packet tracer listening on ports 80 and 443. This feature generates a top 5 list of those who send the most packets and displays their IP addresses. You can create your own HTML file by modifying `formulaire.html`.The purpose of the server is to create a form for selecting the best image and extract the data by storing it in a .csv file. The packet tracer analyzes the packets during their transmission.

# Sioux Server
To launch the server, you can execute the command `make && make start_sioux` to start the server directly on port 8080. To connect to it, please open Firefox and enter the following address: `localhost:8080`. This will allow you to access our pages directly to vote for your favorite image. The results will be stored in a .csv file located in the sioux directory.

# Ablette Server

To launch Ablette, you can execute the command `make && make start` at the root of the project. This will display different TCP packets and sort them in descending order based on the IP addresses that communicate the most with your target. The ranking is displayed every 5 received packets but can be modified by adjusting the code in ablette.c (Frequence_aff). Packets are sorted by ports 80 (http) and 443 (https).

# Cleaning

You can use the command `make clean` to clean all directories in the project.


# Conctact
Thank you for visiting my project. If you have any comments or questions, please feel free to contact me via email at the following address: cruchetsimon@gmail.com
