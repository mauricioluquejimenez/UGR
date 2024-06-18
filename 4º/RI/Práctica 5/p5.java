import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;

import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;

public class busqueda {

    String indexPath = "./index";

    public void indexSearch(Analyzer a, Similarity s){

        //Creación de Directory que lee la ubicación del índice
        Directory dir = FSDirectory.open(Paths.get(indexPath));

        //Creación de Index Reader que lee el índice
        IndexReader reader = DirectoryReader.open(d);

        //Creación de IndexSearcher que calcula la similitud entre los documentos y la consulta
        IndexSearcher searcher = new IndexSearcher(reader);

        //Asignación de medida de similitud a IndexSearcher (BM25)
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
        int th = td.totalHints;

        //Extracción de datos del resultado de la consulta
        //Extracción del ID de cada documento y de los campos
        ScoreDocs[] sd = td.ScoreDoc;
        
        System.out.println(th + "resultados");
        for(ScoreDoc i : ScoreDocs){

            Document d = i.doc;

            /*
             * Extracción de campos como ID, spoken_words, etc.
             */

        }
    }

    public static void main(String[] args){
        
        Analyzer a = new StandardAnalyzer();
        Similarity s = new BM25Similarity();

        indexSearch(a,s);
    }
}