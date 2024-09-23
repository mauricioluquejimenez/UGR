/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package analizador1;

import java.nio.charset.StandardCharsets;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.CharArraySet;
import org.apache.lucene.analysis.LowerCaseFilter;
import org.apache.lucene.analysis.StopFilter;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.Tokenizer;
import org.apache.lucene.analysis.core.WhitespaceTokenizer;
import org.apache.lucene.analysis.ngram.EdgeNGramTokenFilter;
import org.apache.lucene.analysis.snowball.SnowballFilter;
import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import java.io.BufferedWriter;
import org.apache.tika.metadata.Metadata;
import org.apache.tika.parser.AutoDetectParser;
import org.apache.tika.sax.BodyContentHandler;
import org.tartarus.snowball.ext.SpanishStemmer;
import org.xml.sax.SAXException;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;

import java.util.ArrayList;
import java.util.List;

import org.apache.lucene.analysis.synonym.SynonymGraphFilter;
import org.apache.lucene.analysis.synonym.SynonymMap;
import org.apache.lucene.util.CharsRef;
import org.apache.tika.exception.TikaException;

/**
 *
 * @author mauricio
 */
public class Ejercicio3 {
    
    private static void addSynonyms(SynonymMap.Builder builder, String term, String... synonyms){
        for (String synonym : synonyms) {
            builder.add(new CharsRef(term), new CharsRef(synonym), true);
        }
    }
    
    public static Analyzer CustomAnalyzer() {
        Analyzer custom = new Analyzer() {
            @Override
            protected Analyzer.TokenStreamComponents createComponents(String todo) {
                
                Tokenizer source = new WhitespaceTokenizer();

                TokenStream result = source;

                // Eliminar palabras vacías
                CharArraySet stopWords = new CharArraySet(List.of("de","la","en", "es", "una", "y","el", "a", "su"), true);
                result = new StopFilter(result, stopWords);

                //   TokenStream  filter = new LowerCaseFilter(source);
                result = new LowerCaseFilter(result);

                // Agrupar por sinónimos
                SynonymMap.Builder synonymMapBuilder = new SynonymMap.Builder(true);
                try { addSynonyms(synonymMapBuilder, "avanzando", "creciendo"); }
                catch (Exception e) { e.printStackTrace(); }
                
                try { SynonymMap synonymMap = synonymMapBuilder.build(); result = new SynonymGraphFilter(result, synonymMap, true); }
                catch (IOException e) { e.printStackTrace(); }

                // Stemming (reducción a forma raíz)

                /*InputStream affixStream = new FileInputStream("es_ES.aff");
                InputStream dictStream = new FileInputStream("es_ES.dcit");
                Directory dirtemp = FSDirectory.open(Paths.get("/temp"));
                Dictionary d = new Dictionary(dirtemp, "filetemp", affixStream, dictStream);*/
                
                SpanishStemmer spanishStemmer = new SpanishStemmer();
                result = new SnowballFilter(result, spanishStemmer);

                result = new EdgeNGramTokenFilter(result, 3, 3);
                
                return new Analyzer.TokenStreamComponents(source, result);
            }

        };
        return custom;
    }

    public static void main(String [] args ) throws IOException {

        String directorio = args[0];
        File dir = new File(directorio);
        
        if (!dir.isDirectory()) {
            System.err.println("El argumento proporcionado no es un directorio válido." + dir );
            System.exit(1);
        }
        
        File[] archivos = dir.listFiles();
        if (archivos == null || archivos.length == 0) {
            System.out.println("El directorio está vacío.");
            return;
        }

        Analyzer custom = CustomAnalyzer();

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
                        String text = handler.toString();
                        TokenStream tokenStream = custom.tokenStream(null, text);
                        tokenStream.reset();

                        // Usar el tokenStream para recorrer los tokens del documento
                        CharTermAttribute charTermAttribute = tokenStream.addAttribute(CharTermAttribute.class);
                        List<String> WordsList = new ArrayList<>();  

                        while (tokenStream.incrementToken()) {
                            String token = charTermAttribute.toString();
                            // Añade el token a la lista
                            if (!token.isEmpty()) { WordsList.add(token); }
                        }

                        // Escribe los resultados en el archivo CSV
                        for (String i : WordsList) {
                            writer.write(i + ";;" + "\n");
                        }

                    System.out.println("Se ha generado el archivo CSV: " + csvFileName);
                    
                } catch (IOException e) { e.printStackTrace(); }
            }
        }
    }   
}