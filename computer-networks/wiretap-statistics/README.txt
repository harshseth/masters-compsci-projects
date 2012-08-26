Computer Networks Project (Fall 2009)

Project Goal
Programs, tcpdump (http://www.tcpdump.org/) and Wireshark (http://www.wireshark.org/), allow sniffing and analyzing of packets being transmitted or received over a LAN. One prominent use of this information is in troubleshooting network configuration and reachability. In this project, we will provide you with data captured using these tools. Your task is to write the analysis routines similar to those provided by tcpdump and Wireshark. Your program must be written in C/C++ and should work on the Burrow or Sharkestra Linux machines.

Project Specification
Your program, wiretap, should take a file containing tcpdump data as its input and output the statistics detailed later in this document. Since this data is not in human-readable format, you will have to use the Packet Capture Library, libpcap.a, and the functions in its header file, pcap.h (found in /usr/include on the CS Linux machines) to read the data. When compiling your program, include the pcap library by using -lpcap as the first argument to your GNU compiler. For example, gcc -lpcap -o wiretap wiretap.c will compile a C program with the pcap library support. For C++, simply change the compiler from gcc to g++.

The other steps you should follow are:

- Open an input file using function pcap_open_offline(). - Check that the data you are provided has been captured from Ethernet using function pcap_datalink(). - Read packets from the file using function pcap_loop(). Notice that this function needs to be called only once. It takes 4 arguments. Of these, the second and the third arguments are of most interest to you. The second argument lets you specify how many packets to read from the file. The third argument, pcap_handler callback, is where most of the action happens. Here, callback is the function you write to process data from each packet.

- You can pass the callback function to the pcap_loop() function simply by giving its name as the appropriate argument to pcap_loop(). The callback function must be a void function that takes three arguments, of the types u_char , const struct pcap_pkthdr , const u_char . The callback is called by pcap_loop() once for each packet. The second argument to the callback is the special libpcap header, which can be used to extract the entire packet length and the packet arrival time (see the pcap_pkthdr structure in /usr/include/pcap.h). The third argument contains the contents of a single packet (from the Ethernet packet header onward). - Close the file using function pcap_close().

Packet format

Each packet in the file(s) provided to you is in tcpdump format. It contains a tcpdump-specific header, an Ethernet header, followed by network and transport layer headers and their contents. At the network layer, the captures will have IP and other protocols. TCP and UDP will both be present at the transport layer. We expect you to process Ethernet, IP, TCP, and UDP headers. The packets will also contain application data, but you do not need to process those headers. You will need to understand each of the header formats to accomplish this task. You are encouraged to reuse the structures from the relevant header files in the /usr/include/net and /usr/include/netinet directories on the CS Linux machines. These files contain structures used by the Linux operating system for actual packets. They can be used to greatly simplify the process of parsing the packets. However, you are still free to implement the structures on your own, if you wish.

Program output
- The callback function should gather statistics from each packet to enable your program to print the following on standard out:

- Start date and time, total duration, and total number of packets in the packet capture - Unique Ethernet addresses, along with the total number of packets containing each address. Represent Ethernet addresses in hex-colon format. - Unique Network layer protocols seen, and how many packets use them. Report them by protocol number, except for IP. - Unique IP addresses, along with the total number of packets containing each address. - Unique Transport layer protocols seen, and how many packets use them. Report them by protocol number, except for TCP and UDP. - Unique TCP and UDP ports, along with the total number of packets containing each port number. - Determine the number of UDP packets with a correct checksum, an incorrect checksum, and those that omit checksum calculations - For TCP, report the number of packets containing each flag - Average, minimum, and maximum packet sizes. Here, packet refers to everything beyond the tcpdump header. - Average, minimum, and maximum overhead. Here, overhead refers to the Ethernet, network, and transport layer header sizes with respect to the entire packet. Here, packet refers to everything beyond the tcpdump header. You may ignore packets that are not IP or not TCP or UDP in this calculation

Test files

The following are two packet captures you should use for testing purposes.

Packet Capture 1: A traceroute (primarialy UDP traffic)

Sample Output

Packet Capture 2: Downloads of two web sites (TCP traffic with some UDP as well)

Sample Output

Miscellaneous
If you have root access on any machine, install Wireshark and experiment with it. Resource: http://www.tcpdump.org/pcap.htm Use the ntohs and ntohl functions as appropriate to read values that span multiple bytes. This ensures that the bytes are in proper byte order (Endianness) for use at this host.

Extra Credit

For 5% extra credit, infer flow information in your data. Specifically, for each combination seen of source and destination IP addresses and ports, and transport layer protocol, list the number of packets that traveled between them in each direction.

Deliverables and Grading

The grading will be based on the following three deliverables, all of which are due by 11.59pm on the due date:

Project report: Prepare a 2-page report detailing your implementation. For each major component of the project, the report should provide any algorithms and assumptions you made in your program. Also, list any known limitations of your program, including portions that are not implemented or do not work correctly. The report should use a 10-point font, with margins on either of the four sides not exceeding 1 inch. Submit the report in pdf format with your code (more details below). No other format, e.g., doc, will be accepted. Also, print your project report double-sided and bring the hard copy to the demo. Code submission and demo: Submit your code, and project report files as a single archive file (.tar or .tar.gz file formats only) via OnCourse?. The demo slots will be posted by the AIs on the Web board soon after the submission deadline. Schedule an appointment with them to demonstrate your project. Failure to attend your scheduled demonstration timeslot will result in a 10 point reduction in your grade. Each group is required to complete a demonstration in order to receive credit for the project.

Partner evaluation:

Evaluate your project partner. Specifically, comment on their competency, motivation, and contributions to the project. Send your evaluation to the email address. Failure to send this email by the project submission deadline will result in a 10 point reduction in your grade. 
