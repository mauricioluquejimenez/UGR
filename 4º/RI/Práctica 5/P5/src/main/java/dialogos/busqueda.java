package dialogos;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.TotalHits;
import org.apache.lucene.search.similarities.BM25Similarity;
import org.apache.lucene.search.similarities.Similarity;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

public class busqueda {

    String indexPath = "./index";

    public void indexSearch(Analyzer a, Similarity s) throws IOException, ParseException{

        //Creación de Directory que lee la ubicación del índice
        Directory dir = FSDirectory.open(Paths.get(indexPath));

        //Creación de Index Reader que lee el índice
        IndexReader reader = DirectoryReader.open(dir);

        //Creación de IndexSearcher que calcula la similitud entre los documentos y la consulta
        IndexSearcher searcher = new IndexSearcher(reader);

        //Asignación de medida de similitud a IndexSearcher (BM25 con k=1'2 y b = 0'75)
        searcher.setSimilarity(s);

        /*
         * BufferedReader in = new BufferedReader(new InputStreamReader(System.in.StandardCharsets.UTF_8));
         */

        //Creación del parser sobre el que se crea la consulta
        QueryParser parser = new QueryParser("", a);

        //Creación de la consutla usada por el IndexSearcher
        Query q = parser.parse("");

        //Ejecución de la consulta que devuelve un TopDocs
        TopDocs td = searcher.search(q, 50);

        //Extracción de datos del resultado de la consulta
        //Extración del número total de resultados
        TotalHits th = td.totalHits;

        //Extracción de datos del resultado de la consulta
        //Extracción del ID de cada documento y de los campos
        ScoreDoc[] sd = td.scoreDocs;
        
        System.out.println(th + "resultados");
        for(ScoreDoc i : sd){

            Document doc = searcher.doc(i.doc);
            
            //Explicación de cómo se ha obtenido el score de cada documento
            //searcher.explain(q,i.doc.);

             //Extracción de campos como ID, spoken_words, etc.
             int id = doc.getField("episode_id").numericValue().intValue();
             int number = doc.getField("number").numericValue().intValue();
             int timestamp = doc.getField("timestamp_in_ms").numericValue().intValue();
             String character = doc.get("raw_character_text");
             String location = doc.get("raw_location_text");
             String words = doc.get("spoken_words");

        }
    }

    public void main(String[] args) throws IOException{
        
        try {
            
            Analyzer a = new StandardAnalyzer();
            Similarity s = new BM25Similarity();
            
            indexSearch(a,s);
            
        } catch (ParseException ex) { Logger.getLogger(busqueda.class.getName()).log(Level.SEVERE, null, ex); }
    }
}