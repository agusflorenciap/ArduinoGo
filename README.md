<h1>Arduino y Go en entorno distribuido</h1>

<h2>Descripción</h2>
El proyecto consiste en utilizar Golang y Arduino en un entorno distribuido. El Arduino conectado vía USB recolecta información con un sensor ultrasónico. También se dispone de un módulo de Ethernet que facilita su acceso a la red de trabajo.
Los datos recogidos se almacenan en una base de datos MySQL, corriendo en una VM. El servidor web se realizó en Go. En la página web se visualizan las mediciones registradas.

<h2>Lenguaje y entorno</h2>

- <b>Arduino en Arduino IDE</b>
- <b>Go en Visual Studio Code</b>

<h2>Software adicional</h2>

- <b>Oracle VM VirtualBox 6.1</b>
- <b>MySQL 8.0</b>

<h2>Hardware</h2>

- <b> Arduino con módulo Ethernet</b>
- <b> Sensor ultrasónico HC-SR04</b>

