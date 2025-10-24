#include <cstring>
#include "conexion.h"

/*********************************************************************************/
/*                    METODOS usados solo por el SERVIDOR                        */
/*********************************************************************************/

void Conexion::Crear_Servidor(){

#ifdef WINDOWS
    WSADATA wsaData;                // Inicializamos la libreria de socket de windows
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        cout << "Error iniciando Winsock, de windows";
        exit(1);
    }
    else
        cout << "\nIniciado Winsock, de windows \n";
#endif

    if((sock=socket(AF_INET,SOCK_STREAM,0))<0){ // creamos el socket
        cout << "Error creando el socket saliendo" << endl;
        exit(0);
    }
    else
        cout << "Socket Creado con exito." << endl;

    direccion.sin_family= AF_INET; // familia socket
    direccion.sin_addr.s_addr = INADDR_ANY;
//    cout << "direccion ip: " << inet_ntoa(direccion.sin_addr) << endl;
 
    direccion.sin_port=htons(PUERTO); //Asignamos Puerto

    if(bind(sock,(struct sockaddr *)&direccion,sizeof(direccion))<0 ){//bindeamos
        cout << "Error en el bind del socket saliendo" << endl;
        exit(0);
    }
    else
        cout << "Socket bindeado con exito." << endl;

    listen(sock,5);    // Esperaremos con una cola de maximo 5 clientes
}

/*********************************************************************************/

int Conexion::Aceptar_Cliente(){
    if((conectado=accept(sock,0,0))<0){ //Espero un cliente
            cout << "error aceptando connexion" << endl;
            exit(0);
    }
    return conectado;
}

/*********************************************************************************/
/*                    METODOS DE LA Conexion para el CLIENTE                     */
/*********************************************************************************/

void Conexion::Establecer_Conexion(const char *dir_ip){
    cout << "Direccion IP: " << dir_ip << endl;
    char error[200];
#ifdef WINDOWS
    WSADATA wsaData;                // Inicializamos la libreria de socket de windows
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
       cout << "Error iniciando Winsock, de windows" ;
       exit(1);
    }
#endif // WINDOWS

    direccion.sin_addr.s_addr=inet_addr(dir_ip);
    direccion.sin_family=AF_INET;                // protocolo af inet, visto antes.
    direccion.sin_port=htons(PUERTO);            //htons convierte el puerto al formato de red.

    conectado=socket(AF_INET,SOCK_STREAM,0);          // obtenemos el descriptor del socket
    if(conectado < 0){                                //Error
        perror(error);
        cout << error << endl;
        exit(0);
        }
    else
        cout << "socket creado con exito." << endl;

//conectamos al servidor
    if(connect(conectado,(struct sockaddr*)&direccion,sizeof(direccion))<0)
    {
        cout << "error conectando..." << endl;
        exit(0);
    }

    cout << "Conectado al servidor\n";
}

/*********************************************************************************/
/*             METODOS DE LA Conexion para el servidor y el CLIENTE              */
/*********************************************************************************/

/*********************************************************************************/

void Conexion::Enviar_Msg(char *msg){
    if(send(conectado,(char*) msg,strlen(msg),0)<0){ 
        cout << "error enviando msg="<< msg << endl;
        exit(0);
     }
}

/*********************************************************************************/

void Conexion::Recibir_Msg(char *msg){
    memset(buffer,0,sizeof(buffer));           //esta funcion vacia el buffer de entrada
    bytesrecv=recv(conectado,buffer,1024,0);
    if(bytesrecv == -1){
        cout <<"Error recibiendo msg, saliendo...";
        exit(0);
    }
    else
        buffer[bytesrecv]='\0';

    strcpy(msg,buffer);
}

/*********************************************************************************/

void Conexion::Cerrar_Conexion(){
    cout << "Cerrada la conexion" << endl;
#ifdef WINDOWS
    closesocket(conectado);
#endif // WINDOWS
#ifdef LINUX
    close(conectado);
#endif // LINUX    
}




