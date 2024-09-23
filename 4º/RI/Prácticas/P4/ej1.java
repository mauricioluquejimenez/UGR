import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.index.IndexPath;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.search.similarities.BM25Similarity;
import org.apache.lucene.store.Directory;

public class IndiceSimple{

    String IndexPath = "/.index";
    String DocPath = "./DataSet";

    boolean create = true;

    public void configurarIndice(Analyzer analyzer, Similarity similarity) throws IOException{

        //Creación del IndexWriterConfig, encargado de actualizar el índice, con el StandardAnalyzer
        IndexWriterConfig iwc = new IndexWriterConfig(analyzer);

        //Configuración del IndexWriterConfig
        //Modelo de similitud pasado como argumento
        //Modelo de aprtura CREATE_OR_APPEND, que permite crear un índice o abrirlo si ya existe 
        iwc.setSimilarity(similarity).setOpenMode(IndexWriterConfig.OpenMode.CREATE_OR_APPEND);

        //Apertura del directorio que contiene el índice
        Directory dir = FSDirectory.open(Paths.get(indexPath));

        //Creación del IndexWriter, encargado de crear/modificar el índice
        //Directorio e IndexWriterConfig, configurados previamente y pasados como argumento
        IndexWriter writer = new IndexWriter(dir,iwc);

    }

    public void indexarDocumentos(){

        for(elementos d : DocPath){

            String cadena = leerDocumento(d);
            Document doc = new Document();

            //Extracción de subcadena para separar los campos del documento sobre el que se itera
            Integer start /*= ?*/;
            Integer end /*= ?*/;
            String aux = cadena.substring(start,end);
            Integer valor = Integer.decode(aux);

            //Creación de campo específico, asignación de nombre y valor concreto
            //Como se crea un campo que almacena el ID numérico, se añade un IntPoint y un StoreField para poder devolverlo en una consulta
            doc.add(new IntPoint("ID", valor));
            doc.add(new StoreField("ID", valor));

            //Sobreescritura de las variables start y end para crear un nuevo campo
            //start = ?//
            //end = ?//
            valor = Integer.decode(aux);

            //Creación de campos específicos, asignación de nombre y valor concreto
            doc.add(new IntPoint("number", valor));
            doc.add(new StoreField("number", valor));

            //Sobreescritura de las variables start y end para crear un nuevo campo
            //start = ?//
            //end = ?//
            valor = Integer.decode(aux);
            doc.add(new IntPoint("timestamp", valor));
            doc.add(new StoreField("timesatamp", valor))

            //Sobreescritura de las variables start y end para crear un nuevo campo
            //Como el tipo de campo TextField se puede almacenar y devolver en consulta, no es necesario añadir un StoreField
            //start = ?//
            //end = ?//
            cuerpo = cadena.substring(start,end);
            doc.add(new TextField("character", cuerpo, Field.Store.YES));

            //Sobreescritura de las variables start y end para crear un nuevo campo
            //Como el tipo de campo TextField se puede almacenar y devolver en consulta, no es necesario añadir un StoreField
            //start = ?//
            //end = ?//
            cuerpo = cadena.substring(start,end);
            doc.add(new TextField("location", cuerpo, Field.Store.YES));

            //Sobreescritura de las variables start y end para crear un nuevo campo
            //Como el tipo de campo TextField se puede almacenar y devolver en consulta, no es necesario añadir un StoreField
            //start = ?//
            //end = ?//
            cuerpo = cadena.substring(start,end);
            doc.add(new TextField("words", cuerpo, Field.Store.YES));

            //Si el documento no existe, inserción en el índice
            //Si existe, actualización del mismo
            if(Paths.get(indexPath).exists()) writer.addDocument(doc);
            else writer.updateDocument(new Term("ID", valor.toString()), doc);
            
        }
    }

    public void close(){

        try{

            writer.commit();
            writer.close();

        } catch (IOException e){ System.out.println("Error closing the index."); }

    }

    public static void main(String[] args){

        //Analizador estándar
        Analyzer analyzer = new StandardAnalyzer();

        //Modelo de similitud estándar, superior al ClassicSimilarity
        Similarity similarity = new BM25Similarity();

        //Creación del índice
        IndiceSimple index = IndiceSimple();

        //Configuración del índice con el analizador y el modelo de similitud indicados
        index.configurarIndice(analyzer,similarity);
        index.indexarDocumentos();
        index.close();

    }

}