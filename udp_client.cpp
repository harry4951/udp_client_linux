/** @file simple_client.c
 *
 * @brief This simple client demonstrates the most basic features of JACK
 * as they would be used by many applications.
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>

#include <jack/jack.h>

#define NUM_SAMPLES 1024
#define BUFFER_SIZE	256
#define N_smallpacket 4

void error(const char *);

typedef struct 
{
  int packet_id;
  int packet_id_num;
  float buffer[BUFFER_SIZE];
  //float *buffer;
  
}netpacket;

int sock;
unsigned int length;
struct sockaddr_in server;
int flag = 0;

jack_port_t *input_port1;
jack_port_t *input_port2;
jack_port_t *output_port1;
jack_port_t *output_port2;
jack_client_t *client;

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by 
 * the user (e.g. using Ctrl-C on a unix-ish operating system)
 */
int
process (jack_nframes_t nframes, void *arg)
{
	jack_default_audio_sample_t *in1, *in2, *out1, *out2;
	netpacket *netpacket_test = (netpacket*)arg;
	//netpacket_test->buffer = malloc(sizeof(jack_default_audio_sample_t) * nframes );
//	static int i = 0;
//	netpacket_test -> packet_id = 0 ;
//	netpacket_test -> packet_id_num = i ;
//	i++;
//	if(i == N_smallpacket) 
//	{
//		i = 0;
//	}
	//netpacket_test -> buffer[0] = 1.0;
	//netpacket_test -> buffer[1] = 1.0;
	//for(int j = 0; j < BUFFER_SIZE; j++)
	//{
	//	netpacket_test -> buffer[j] = 1.0;
	//}
	//printf("buffer size %n", times);
	//sizeof(netpacket_test.buffer));

	in1 = (jack_default_audio_sample_t*)jack_port_get_buffer (input_port1, nframes);
	//in2 = jack_port_get_buffer (input_port2, nframes);
	//out1 = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port1, nframes);
	//out2 = jack_port_get_buffer (output_port2, nframes);
	//memcpy (out1, in1,
	//	sizeof (jack_default_audio_sample_t) * nframes);
	memcpy (netpacket_test->buffer, in1,
		sizeof (float) * BUFFER_SIZE);
	flag = 1;
	//memcpy (netpacket_test->buffer, in1,
	//	sizeof (jack_default_audio_sample_t) * nframes);
	//memcpy (out1, in1,
	//	sizeof (jack_default_audio_sample_t) * nframes);
//	int sent_bytes = sendto(sock, (const void*) netpacket_test,
//                  sizeof(netpacket),0,(const struct sockaddr *)&server,length);
//  		if (sent_bytes < 0)
//  		{
//  		   	error("Sendto");
//  		}

	return 0;      
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void
jack_shutdown (void *arg)
{
	exit (1);
}

int
main (int argc, char *argv[])
{
	const char **ports;
	const char *client_name = "simple";
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;
	netpacket *netpacket_test;
	netpacket_test=(netpacket*)malloc(sizeof(netpacket));
	/* open a client connection to the JACK server */

	client = jack_client_open (client_name, options, &status, server_name);
	if (client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK server started\n");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		fprintf (stderr, "unique name `%s' assigned\n", client_name);
	}
//////////////////////////////////////////////
//	 int sock;
//     unsigned int length;
//     struct sockaddr_in server;
     struct hostent *hp;

     // Parse commmand line arguments
     if (argc != 3) { printf("Usage: server port\n");
                      exit(1);
     }

     // Prepare Socket Connection
     sock= socket(AF_INET, SOCK_DGRAM, 0);
     if (sock < 0) error("socket");

     server.sin_family = AF_INET;
     hp = gethostbyname(argv[1]);
     if (hp==0) error("Unknown host");

     bcopy((char *)hp->h_addr, 
          (char *)&server.sin_addr,
           hp->h_length);
     server.sin_port = htons(atoi(argv[2]));
     length=sizeof(struct sockaddr_in);
/*
     // Prepare input wavedata with a linear ramp
    float wavedata[NUM_SAMPLES];
    //int flag_number = 0;
    int value = 0;
	static const float DELTA = 0.1f;

    for(int i=0; i<NUM_SAMPLES; i++) {
          wavedata[i] = DELTA * value;
          value++;
      }

    // Loop over signal in chunks of BUFFER_SIZE and send each buffer to socket
	//Every time send 32 floating number
	
	int N_BUFFERS = NUM_SAMPLES / BUFFER_SIZE/N_smallpacket;
  //netpacket netpacket_test;
  for(int big_buf_idx=0; big_buf_idx < N_BUFFERS; big_buf_idx++)
  {
    netpacket_test.packet_id = big_buf_idx;
    //printf("netpacket_test.packet_id %d \n", netpacket_test.packet_id);
    for(int buf_idx=0; buf_idx<N_smallpacket; buf_idx++)
    {

      //netpacket_test.packet_id_num = RandomId();
      netpacket_test.packet_id_num = buf_idx;
      //printf("netpacket_test.packet_id_num %d \n", netpacket_test.packet_id_num);
      memcpy(netpacket_test.buffer, &wavedata[(big_buf_idx * N_smallpacket + netpacket_test.packet_id_num) * BUFFER_SIZE], BUFFER_SIZE * sizeof(float));
      //test timeout function at receiver
      //if(big_buf_idx!=0 && big_buf_idx!=1)
      //{
      //  if(buf_idx==3)
      //  {
      //    usleep(100*1000);  //50ms
      //  }
      //}
      //for(int i=0; i< BUFFER_SIZE; i++)
      //{
      //  printf("%5.2f\n",netpacket_test.buffer[i]);
      //}

      int sent_bytes = sendto(sock, (const void*) &netpacket_test,
                  sizeof(netpacket_test),0,(const struct sockaddr *)&server,length);
  		if (sent_bytes < 0)
  		{
  		   	error("Sendto");
  		}

  		usleep(1);

    }

  }*/
////////////////////////////////////////////////
	/* tell the JACK server to call `process()' whenever
	   there is work to be done.
	*/

	//jack_set_process_callback (client, process, 0);
jack_set_process_callback (client, process, netpacket_test);
	/* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (client, jack_shutdown, 0);

	/* display the current sample rate. 
	 */

	printf ("engine sample rate: %" PRIu32 "\n",
		jack_get_sample_rate (client));

	/* create two ports */

	input_port1 = jack_port_register (client, "input1",
					 JACK_DEFAULT_AUDIO_TYPE,
					 JackPortIsInput, 0);
	input_port2 = jack_port_register (client, "input2",
					 JACK_DEFAULT_AUDIO_TYPE,
					 JackPortIsInput, 0);
	output_port1 = jack_port_register (client, "output1",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);
	output_port2 = jack_port_register (client, "output2",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);

	if ((input_port1 == NULL) || (output_port1 == NULL) || 
	(input_port2 == NULL) || (output_port2 == NULL)) {
		fprintf(stderr, "no more JACK ports available\n");
		exit (1);
	}

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

	/* Connect the ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
	 */
/*
	ports = jack_get_ports (client, NULL, NULL,
				JackPortIsPhysical|JackPortIsOutput);
	if (ports == NULL) {
		fprintf(stderr, "no physical capture ports\n");
		exit (1);
	}

	if (jack_connect (client, ports[0], jack_port_name (input_port1))) {
		fprintf (stderr, "cannot connect input ports\n");
	}

	free (ports);
	
	ports = jack_get_ports (client, NULL, NULL,
				JackPortIsPhysical|JackPortIsInput);
	if (ports == NULL) {
		fprintf(stderr, "no physical playback ports\n");
		exit (1);
	}

	if (jack_connect (client, jack_port_name (output_port1), ports[0])) {
		fprintf (stderr, "cannot connect output ports\n");
	}

	free (ports);
*/
	/* keep running until stopped by the user */

	//sleep (-1);
	while(1)
	{
		if(flag == 1)
		{
			int sent_bytes = sendto(sock, (const void*) netpacket_test,
                 sizeof(netpacket),0,(const struct sockaddr *)&server,length);
	  		flag = 0;
	  		if (sent_bytes < 0)
	  		{
	  		   	error("Sendto");
	  		}
		}
	}

	/* this is never reached but if the program
	   had some other way to exit besides being killed,
	   they would be important to call.
	*/

	jack_client_close (client);
	exit (0);

}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
