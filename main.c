#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")


// Función que selecciona una vocal aleatoria
char obtenerVocal() {
    char vocales[] = {'a', 'e', 'i', 'o', 'u'};
    return vocales[rand() % (sizeof(vocales) / sizeof(vocales[0]))];
}

// Función que selecciona una consonante aleatoria
char obtenerConsonante() {
    char consonantes[] = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};
    return consonantes[rand() % (sizeof(consonantes) / sizeof(consonantes[0]))];
}

// Función que genera una cadena alternando entre vocales y consonantes
void generarUsuario(char *cadena, int longitud) {

    // Elegir al azar si empezar por vocal o consonante
    int esVocal = rand() % 2;

    for (int i = 0; i < longitud; i++) {
        if (esVocal) {
            cadena[i] = obtenerVocal();
        } else {
            cadena[i] = obtenerConsonante();
        }
        esVocal = !esVocal; // Alternar entre vocal y consonante
    }

    cadena[longitud] = '\0'; // Añadir el carácter nulo al final de la cadena
}


// Función que genera un carácter alfanumérico aleatorio
char obtenerAlfanumerico() {
    int tipo = rand() % 3; // 0: minúscula, 1: mayúscula, 2: número
    if (tipo == 0) {
        return 'a' + rand() % 26; // Letra minúscula
    } else if (tipo == 1) {
        return 'A' + rand() % 26; // Letra mayúscula
    } else {
        return '0' + rand() % 10; // Número
    }
}

// Función que genera una cadena alfanumérica con al menos una minúscula, una mayúscula y un número
void generarContrasenia(char *cadena, int longitud) {


    // Generar al menos una letra minúscula, una mayúscula y un número
    cadena[0] = 'a' + rand() % 26; // Letra minúscula
    cadena[1] = 'A' + rand() % 26; // Letra mayúscula
    cadena[2] = '0' + rand() % 10;  // Número

    // Llenar el resto de la cadena con caracteres aleatorios
    for (int i = 3; i < longitud; i++) {
        cadena[i] = obtenerAlfanumerico();
    }

    // Mezclar la cadena para que los caracteres obligatorios no estén en las primeras posiciones
    for (int i = 0; i < longitud; i++) {
        int j = rand() % longitud;
        char temp = cadena[i];
        cadena[i] = cadena[j];
        cadena[j] = temp;
    }

    cadena[longitud] = '\0'; // Añadir el carácter nulo al final de la cadena
}





int main()
{

    //inicializacion de winsocket
    WSADATA WsaData;
    WORD  wVersionRequerida = MAKEWORD(2, 2);
    WSAStartup(wVersionRequerida, &WsaData);

    //creacion del socket
    SOCKET SockEscucha;
    SockEscucha = socket(AF_INET, SOCK_STREAM, 0);

    //especificar la direcion de la red
    SOCKADDR_IN DireccionLocal;
    memset(&DireccionLocal, 0, sizeof(DireccionLocal));
    DireccionLocal.sin_family = AF_INET;
    DireccionLocal.sin_port = htons(5000);

    //asociar la direccion al socket
    bind(SockEscucha, (SOCKADDR*)&DireccionLocal, sizeof(DireccionLocal));

    //inciar espera de conexiones al socket y su cantidad maxima de conexiones
    int iResult = listen(SockEscucha, 5);

    //socket que acepta las conecciones
    SOCKET SockConexion;
    SockConexion = accept(SockEscucha, NULL, NULL);


    closesocket(SockEscucha); //cerrar el socket de escucha

    while(1){



    int opcion_menu=0;
    int cantidad_caracteres=0;
    recv(SockConexion, (char*)&opcion_menu, sizeof(opcion_menu), 0);
    recv(SockConexion, (char*)&cantidad_caracteres, sizeof(cantidad_caracteres), 0);

    //printf("opcion %d canitdad %d\n",opcion_menu, cantidad_caracteres);

    char* respuesta;
    if(opcion_menu==1){

        if(cantidad_caracteres<5 || cantidad_caracteres>15){
            respuesta="Error!! la cantidad de caracteres debe ser mayor a 5 y menos que 15";
            printf("%s\n",respuesta);
            send(SockConexion, respuesta, strlen(respuesta), 0);
        }else{
        char usuario[16];
         memset(usuario, 0, sizeof(usuario));
        generarUsuario(usuario,cantidad_caracteres);
        printf("Usuario generado: %s\n",usuario);
        send(SockConexion, usuario, strlen(usuario), 0);
        }
    }else{
        if(opcion_menu==2){

            if(cantidad_caracteres<=8 || cantidad_caracteres>50){
            respuesta="Error!! la cantidad de caracteres debe ser mayor o igual a 8 y menor a 50";
            printf("%s\n",respuesta);
            send(SockConexion, respuesta, strlen(respuesta), 0);
            }
        else{

            char contrasenia[51];
            memset(contrasenia, 0, sizeof(contrasenia));
            generarContrasenia(contrasenia,cantidad_caracteres);
            printf("Contraseña generada: %s\n",contrasenia);
            send(SockConexion, contrasenia, strlen(contrasenia), 0);
            }
    }else{
        break;
    }
    }


}

system("pause");



    closesocket(SockConexion); //cierra el socket de conexion

    WSACleanup(); //cierra la conexion

    return 0;
}
