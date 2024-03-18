package main

import (
	"database/sql"  // interactuar con bd
	"fmt"           // imprimir mensajes
	"log"           //para escribir los registros
	"net/http"      //peticiones http
	"text/template" //para manejar los archivos

	_ "github.com/go-sql-driver/mysql" // librería que permite conectar a MySQL
)

type Medida struct {
	IdDistancia, Distancia int
	Fecha                  string
}

func obtenerBaseDeDatos() (db *sql.DB) {
	usuario := "root"
	pass := "hola"
	host := "tcp(192.168.0.6:3306)"
	nombreBaseDeDatos := "ultrasonico"
	//tiene q tener la forma usuario:contraseña@protocolo(host:puerto)/nombreBaseDeDatos
	db, err := sql.Open("mysql", fmt.Sprintf("%s:%s@%s/%s", usuario, pass, host, nombreBaseDeDatos))
	if err != nil {
		panic(err.Error())
	}
	return db
}

//vistas (templates)
var tmpl = template.Must(template.ParseGlob("vista/*"))

//SELECT
//pág para mostrar lista de registros

func Index(w http.ResponseWriter, r *http.Request) {

	db := obtenerBaseDeDatos()
	filas, err := db.Query("SELECT iddistancia, fecha, distancia FROM distancia ORDER BY iddistancia")

	if err != nil {
		panic(err.Error())
	}

	// si se llega acá, significa que no ocurrió ningún error
	defer filas.Close()

	// acá se mapea lo que traiga la consulta en el for de más abajo
	var m Medida
	medidas := []Medida{}

	// recorrer todas las filas en un "for"
	for filas.Next() {
		err = filas.Scan(&m.IdDistancia, &m.Fecha, &m.Distancia)
		// al escanear puede haber un error
		if err != nil {
			panic(err.Error())
		}
		// y si no, entonces se agrega lo leído al array
		medidas = append(medidas, m)
	}

	tmpl.ExecuteTemplate(w, "Index", medidas)
	defer db.Close()
}

//pág para mostrar registros de forma individual (no terminada en HTML así que no se ve)

func Show(w http.ResponseWriter, r *http.Request) {
	db := obtenerBaseDeDatos()
	nId := r.URL.Query().Get("iddistancia")
	filas, err := db.Query("SELECT * FROM distancia WHERE id=? ", nId)
	if err != nil {
		panic(err.Error())
	}

	defer filas.Close()

	var m Medida

	for filas.Next() {
		err = filas.Scan(&m.IdDistancia, &m.Fecha, &m.Distancia)
		if err != nil {
			panic(err.Error())
		}
	}
	tmpl.ExecuteTemplate(w, "Show", m)
	defer db.Close()

}

func main() {
	log.Println(("Servidor funcionando en http://localhost:8080"))
	http.HandleFunc("/", Index)
	http.HandleFunc("/show", Show)
	http.ListenAndServe(":8080", nil)
}
