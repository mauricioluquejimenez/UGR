/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package analizador1;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.analysis.LowerCaseFilter;
import org.apache.lucene.analysis.StopFilter;
import org.apache.lucene.analysis.snowball.SnowballFilter;
/*import org.apache.lucene.analysis.shingle.ShingleFilter;
import org.apache.lucene.analysis.ngram.EdgeNGramTokenFilter;
import org.apache.lucene.analysis.ngram.NGramTokenFilter;*/
import org.apache.lucene.analysis.synonym.SynonymGraphFilter;
import org.apache.lucene.analysis.synonym.SynonymMap;
import org.tartarus.snowball.ext.SpanishStemmer;

import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import org.apache.lucene.analysis.CharArraySet;
import org.apache.lucene.analysis.commongrams.CommonGramsFilter;
import org.apache.lucene.util.CharsRef;
import org.apache.tika.exception.TikaException;
import org.apache.tika.metadata.Metadata;
import org.apache.tika.parser.AutoDetectParser;
import org.apache.tika.sax.BodyContentHandler;
import org.xml.sax.SAXException;


public class Ejercicio2 {
    private static void addSynonyms(SynonymMap.Builder builder, String term, String... synonyms) throws Exception {
        for (String synonym : synonyms) {
            builder.add(new CharsRef(term), new CharsRef(synonym), true);
        }
    }
    
    public static void main(String[] args) throws IOException, Exception {        
        
        String directorio = args[0];
        File dir = new File(directorio);
        File[] archivos = dir.listFiles();
        
        for (File archivo : archivos) {
            if (archivo.isFile()) {
                // Crea el nombre del archivo CSV usando el nombre del archivo original
                String csvFileName = archivo.getName() + ".csv";

                // Crea un archivo CSV para escribir los resultados
                File csvFile = new File(csvFileName);

                try (FileWriter fileWriter = new FileWriter(csvFile, StandardCharsets.UTF_8);
                 BufferedWriter writer = new BufferedWriter(fileWriter)) {
                 
                 
                InputStream input = new FileInputStream(archivo);
                Metadata metadata = new Metadata();
                BodyContentHandler handler = new BodyContentHandler();
                AutoDetectParser parser = new AutoDetectParser();

                try {
                    parser.parse(input, handler, metadata);
                } catch (SAXException | TikaException ex) {
                }

                // Obtén el contenido del documento como texto
                String texto = handler.toString();

                // Crear un analizador con una lista de palabras vacías personalizada
                CharArraySet stopWords = new CharArraySet(List.of("de","la","en", "es", "una", "y","el", "a", "su"), true);
                try (Analyzer analyzer = new StandardAnalyzer(stopWords)) {
                    // TokenStream con todos los filtros y analizadores
                    TokenStream tokenStream = analyzer.tokenStream(null, new StringReader(texto));

                    // Aplicar los filtros
                    tokenStream = new LowerCaseFilter(tokenStream); // Convierte a minúsculas
                    tokenStream = new StopFilter(tokenStream, stopWords); // Elimina palabras vacías

                    // Filtros de términos comunes
                    List<String> commonTermsList = new ArrayList<>();
                    commonTermsList.add("inteliia artificial");
                    CharArraySet commonTerms = new CharArraySet(commonTermsList, true);
                    tokenStream = new CommonGramsFilter(tokenStream, commonTerms);

                    // Sinónimos
                    // Configuración de sinónimos 
                    SynonymMap.Builder synonymMapBuilder = new SynonymMap.Builder(true);
                    addSynonyms(synonymMapBuilder, "avanzando", "creciendo");
                    SynonymMap synonymMap = synonymMapBuilder.build();
                    tokenStream = new SynonymGraphFilter(tokenStream, synonymMap, true);

                    // Stemming (reducción a forma raíz)
                    SpanishStemmer spanishStemmer = new SpanishStemmer();
                    tokenStream = new SnowballFilter(tokenStream, spanishStemmer);

                    // Shingles (n-gramas)
                    //tokenStream = new ShingleFilter(tokenStream, 2, 2); // 2-gramas (bi-gramas)

                    // Edge N-Gramas
                    //tokenStream = new EdgeNGramTokenFilter(tokenStream, 1, 20,true); // N-gramas de bordes

                    // N-Gramas
                    //tokenStream = new NGramTokenFilter(tokenStream, 1, 3,true); // 1-gramas a 3-gramas

                    // Obtener y mostrar los términos resultantes
                    CharTermAttribute charTermAttribute = tokenStream.addAttribute(CharTermAttribute.class);
                    tokenStream.reset();

                    while (tokenStream.incrementToken()) {
                        String token = charTermAttribute.toString();
                        writer.write(token + "\n");
                    }

                    // Cerrar el tokenStream
                    tokenStream.close();
                }

                System.out.println("Se ha generado el archivo CSV con la frecuencia de términos: " + csvFileName);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }     
}