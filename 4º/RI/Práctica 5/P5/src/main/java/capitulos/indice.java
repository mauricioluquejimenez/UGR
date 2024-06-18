package capitulos;

import com.opencsv.CSVParserBuilder;
import com.opencsv.CSVParser;
import com.opencsv.CSVReader;
import com.opencsv.CSVReaderBuilder;
import com.opencsv.exceptions.CsvValidationException;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.search.similarities.BM25Similarity;
import org.apache.lucene.search.similarities.Similarity;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

public class indice {

    String indexPath = "/home/mauricio/Indices";
    String DocPath = "/home/mauricio/CapitulosUnidos";
    IndexWriter writer;

    public void configurarIndice(Analyzer analyzer, Similarity similarity) throws IOException {

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
        writer = new IndexWriter(dir, iwc);

    }

    public String readFileToString(String filePath) throws IOException {
        return new String(Files.readAllBytes(Paths.get(filePath)));
    }

    public void indexarDocumentos() {

        //Columna 0 -> episode_id; columna 1 -> spoken_words; columna 2 -> raw_character_text; columna 3 -> imdb_rating; columna 4 -> imdb_votes
        //Columna 5 -> number_in season; Columna 6 -> original_air_date; Columna 7 -> original_air_year; Columna 8 -> season; Columna 9-> title;
        //Columna 10 -> us_viewers_in_million; Columna 11 -> views
        // Procesar archivos en la carpeta "AllCapitules"
        File allCapitulesFolder = new File(DocPath);
        for (File file : allCapitulesFolder.listFiles()) {
            if (file.isFile() && file.getName().endsWith(".csv")) {
                try {
                    Document doc = new Document();
                    
                    CSVParser csvParser = new CSVParserBuilder()
                            .withSeparator(',')
                            .withIgnoreQuotations(false)
                            .build();

                    // Lee la primera línea (encabezados) y descártala
                    try (CSVReader reader = new CSVReaderBuilder(new FileReader(file))
                            .withCSVParser(csvParser)
                            .build()) {
                        // Lee la primera línea (encabezados) y descártala
                        reader.readNext();
                        
                        // Lee la segunda línea (datos)
                        String data[] = reader.readNext();
                        
                        if (data == null || data.length == 0) {
                            System.out.println("Archivo vacío: " + file.getName());
                            continue; // Salta al siguiente archivo
                        }
                       
                        doc.add(new TextField("episode_id", data[1], Field.Store.YES));
                        doc.add(new TextField("spoken_words", data[2], Field.Store.YES));
                        doc.add(new TextField("raw_character_text", data[3], Field.Store.YES));
                        doc.add(new TextField("imdb_rating",data[4], Field.Store.YES));
                        doc.add(new TextField("imdb_votes", data[5], Field.Store.YES));
                        doc.add(new TextField("number_in_season", data[6], Field.Store.YES));
                        doc.add(new TextField("original_air_date", data[7], Field.Store.YES));
                        doc.add(new TextField("original_air_year", data[8], Field.Store.YES));
                        doc.add(new TextField("season", data[9], Field.Store.YES));
                        doc.add(new TextField("title", data[10], Field.Store.YES));
                        doc.add(new TextField("us_viewers_in_million", data[11], Field.Store.YES));
                        doc.add(new TextField("views", data[12], Field.Store.YES));
                    }
                    writer.addDocument(doc);
                    System.out.println("Índice creado");
                    
                } catch (IOException e) {
                } catch (CsvValidationException ex) {
                    Logger.getLogger(indice.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }         
    }            

    public void close() {

        try {

            writer.commit();
            writer.close();

        } catch (IOException e) {
            System.out.println("Error closing the index.");
        }

    }

    public static void main(String[] args) throws IOException {

        //Analizador estándar
        Analyzer analyzer = new StandardAnalyzer();

        //Modelo de similitud estándar, superior al ClassicSimilarity
        Similarity similarity = new BM25Similarity();

        //Creación del índice
        indice index = new indice();

        //Configuración del índice con el analizador y el modelo de similitud indicados
        index.configurarIndice(analyzer, similarity);
        index.indexarDocumentos();
        index.close();

    }

}