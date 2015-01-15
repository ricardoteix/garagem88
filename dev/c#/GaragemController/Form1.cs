using HttpServer;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GaragemController
{
    public partial class Garagem : Form
    {

        private const String ON = "#on";
        private const String OFF = "#of";
        private const String OPEN = "#op";
        private const String CLOSE = "#cl";
        private const String DONE = "#do";

        private Socket socket;
        private Thread thread;

        String RxString;

        private NetworkStream networkStream;
        private BinaryWriter binaryWriter;
        private BinaryReader binaryReader;

        private Boolean locked = false;
         
        public Garagem()
        {
            InitializeComponent();
            thread = new Thread(new ThreadStart(RunServer));
            thread.Start();

        }
 
        public void RunServer()
        {
            TcpListener tcpListener;
            try
            {
                IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Parse("192.168.1.5"), 2001);
                tcpListener = new TcpListener(ipEndPoint);
                tcpListener.Start();
 
                MessageBox.Show("Servidor habilitado e escutando porta...", "Server App");
 
                socket = tcpListener.AcceptSocket();
                networkStream = new NetworkStream(socket);
                binaryWriter = new BinaryWriter(networkStream);
                binaryReader = new BinaryReader(networkStream);

                //MessageBox.Show("Conexão recebida!", "Server App");
                RxString = ("Conexão recebida\n");
                this.Invoke(new EventHandler(DisplayText));
                binaryWriter.Write("\nconexão efetuada!");

                this.initSerialPort();
 
                string messageReceived = "";

                if (this.locked)
                {
                    binaryWriter.Write("#locked");
                }

                do 
                {
                    messageReceived = binaryReader.ReadString();
                    switch (messageReceived)
                    {
                        case ON:
                            {
                                RxString = ("Mensagem: ON\n");
                                this.Invoke(new EventHandler(DisplayText));
                                this.locked = true;
                                serialPort1.Write("o");
                                break;
                            }

                        case OFF:
                            {
                                RxString = ("Mensagem: OFF\n");
                                this.Invoke(new EventHandler(DisplayText));
                                this.locked = true;
                                serialPort1.Write("f");
                                break;
                            }

                        case OPEN:
                            {
                                RxString = ("Mensagem: OPEN\n");
                                this.Invoke(new EventHandler(DisplayText));
                                serialPort1.Write("p");
                                break;
                            }

                        case CLOSE:
                            {
                                RxString = ("Mensagem: CLOSE\n");
                                this.Invoke(new EventHandler(DisplayText));
                                this.locked = true;
                                serialPort1.Write("c");
                                break;
                            }
                    }
                    
 
                } while (socket.Connected);
            } 
            catch (Exception ex) 
            {
                MessageBox.Show(ex.Message);
            } 
            finally 
            {
                binaryReader.Close();
                binaryWriter.Close();
                networkStream.Close();
                socket.Close();

                this.closeSerialPort();
 
                MessageBox.Show("conexão finalizada", "Server App");
            }
        }

        private void serialPort1_DataReceived (object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            RxString = serialPort1.ReadExisting();
            if (RxString == DONE)
            {
                 //MessageBox.Show("Mensagem: DONE", "Server App");
                 
                 this.locked = false;
            }
            
            this.Invoke(new EventHandler(DisplayText));
        }

        private void DisplayText(object sender, EventArgs e)
        {
           //textBox1.AppendText(RxString);
           richTextBox1.AppendText (RxString);
        }

        private void initSerialPort()
        {
            if (!serialPort1.IsOpen)
            {
                serialPort1.PortName = "COM8";
                serialPort1.BaudRate = 9600;

                serialPort1.Open();

                if (serialPort1.IsOpen)
                {
                    RxString = ("initSerialPort\n");
                    this.Invoke(new EventHandler(DisplayText));
                }
            }
        }

        private void startSerialPort()
        {

        }

        private void closeSerialPort()
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
            }
        }
        
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen) serialPort1.Close();
        }
 
        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void btnSendMsg_Click(object sender, EventArgs e)
        {
            try
            {
                binaryWriter.Write("Server respondendo: Houston, we have a problem!!!");
            }
            catch (SocketException socketEx)
            {
                MessageBox.Show(socketEx.Message, "Erro");
            }
        }
    }
}
