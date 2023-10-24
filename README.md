# Sistema de control de puntos de acceso

### [Enlaces](/Diseños,%20Reportes,%20etc/Enlaces.md)

### [Sensores y materiales](/Diseños,%20Reportes,%20etc/Sensores%20y%20materiales.md)

### Problemática

Los organizadores de carreras necesitan llevar un control de puntos por los que pasa un corredor, conocer el tiempo exacto por el cual pasó un corredor y quien fue, para mantener control y seguridad en las carreras, la distancia de los puntos varía en km principalmente, pero en zonas comúnmente remotas en las que no se tiene acceso a internet o para evitar uso de datos excesivo, además se desea orientar al corredor con un mapa accesible por el punto.

### Lo que se quiere alcanzar

El sistema se basará en nodos interconectados mediante radiofrecuencias los cuales transmitirán las señales de llegada de cada corredor hasta la meta o el punto central, estas señales serán leídas mediante un lector RFID para garantizar y reconocer quién fue el que llegó al punto de acceso.
Solo el punto central o la meta se pretende que esté conectado a internet para mandar señales al servidor web de la nube, pero si este no es el caso también se quiere tener un mecanismo de acceso meramente local en caso de no tener señal.
Además, en cada punto de control se espera poder escanear un mapa para que el corredor conozca su ubicación actual.

### Objetivo

Desarrollar en lenguaje C++ un prototipo de un sistema embebido para la detección de señales RFID, específicamente, pulseras con las que contarán corredores y de esta manera podemos mantener un control de cada uno de los corredores y poder evitar actos de trampas. El prototipo final en C++ correrá en un microcontrolador para verificar dicho objetivo, así mismo se utilizarán sensores RFID como se menciona anteriormente y antenas de radio frecuencia para el envío de datos al dispositivo que se encuentre conectado a internet y poder subir esos datos a un servidor web.

### Puntos de control

![Puntos de control](Diseños,%20Reportes,%20etc/images/Puntos%20de%20control.png)
