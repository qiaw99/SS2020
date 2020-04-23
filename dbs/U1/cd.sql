PREFIX : <http://www.recshop.fake/cd#>
PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>
PREFIX owl: <http://www.w3.org/2002/07/owl#>
PREFIX xsd: <XML Schema>
PREFIX vocab: <http://localhost:2020/resource/vocab/>
PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>
PREFIX map: <http://localhost:2020/resource/#>
PREFIX db: <http://localhost:2020/resource/>

SELECT ?n WHERE {
  ?s rdf:type :cd.
  ?s :year 1985.
  ?s :country USA.
  ?s :price ?n.
  FILTER(?n >= 10)
}
