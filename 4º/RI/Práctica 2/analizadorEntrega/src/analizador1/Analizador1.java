/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package analizador1;

/*import java.io.InputStreamReader;
import java.io.Reader;*/
import org.apache.lucene.analysis.Analyzer;
/*import org.apache.lucene.analysis.CharArraySet;*/
import org.apache.lucene.analysis.LowerCaseFilter;
/*import org.apache.lucene.analysis.StopFilter;*/
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.Tokenizer;
import org.apache.lucene.analysis.classic.ClassicTokenizerFactory;
import org.apache.lucene.analysis.core.LowerCaseFilterFactory;
import org.apache.lucene.analysis.core.SimpleAnalyzer;
import org.apache.lucene.analysis.core.WhitespaceTokenizer;
import org.apache.lucene.analysis.custom.CustomAnalyzer;
import org.apache.lucene.analysis.ngram.EdgeNGramTokenFilter;
import org.apache.lucene.analysis.pattern.PatternReplaceCharFilterFactory;
/*import org.apache.lucene.analysis.snowball.SnowballFilter;*/
import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import java.io.BufferedWriter;
import org.apache.tika.metadata.Metadata;
import org.apache.tika.parser.AutoDetectParser;
import org.apache.tika.sax.BodyContentHandler;
/*import org.tartarus.snowball.ext.SpanishStemmer;/* */

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
/*import java.util.logging.Level;
import java.util.logging.Logger;*/
import java.util.stream.Collectors;

/*import org.apache.lucene.analysis.synonym.SynonymGraphFilter;
import org.apache.lucene.analysis.synonym.SynonymMap;
import org.apache.lucene.util.CharsRef;*/
import org.apache.tika.exception.TikaException;
import org.xml.sax.SAXException;

/**
 *
 * @author jhg
 */
public class Analizador1 {

    public static Analyzer crearAnalyzer() throws IOException {
        Analyzer lineaAnalyzer = CustomAnalyzer.builder()
                .addCharFilter(PatternReplaceCharFilterFactory.NAME, "pattern", "[/(/)/{/};]", "replacement", "X")
                .withTokenizer(ClassicTokenizerFactory.NAME)
                .addTokenFilter(LowerCaseFilterFactory.NAME)
                .build();
        System.out.println(lineaAnalyzer.getClass());
        return lineaAnalyzer;

    }

    public static Analyzer crearAnalyzer2() {
        Analyzer mi_analizador = new Analyzer() {
            @Override
            protected Analyzer.TokenStreamComponents createComponents(String todo) {
                Tokenizer source = new WhitespaceTokenizer();

                TokenStream filter = source; // new ClassicFilter(source);

                //
                filter = new EdgeNGramTokenFilter(filter, 3, 3);
                //   TokenStream  filter = new LowerCaseFilter(source);
                filter = new LowerCaseFilter(filter);
                return new Analyzer.TokenStreamComponents(source, filter);
            }

            /*protected TokenStream normalizar(TokenStream in) {

                return new LowerCaseFilter(in);
            }*/
        };
        return mi_analizador;
    }

    /**
     * @param args the command line arguments
     * @throws java.io.IOException
     */
    public static void main(String[] args) throws IOException {
        
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
        
        Analyzer analyzer1 = new SimpleAnalyzer();
        Analyzer analyzer2 = crearAnalyzer();
        Analyzer analyzer3 = crearAnalyzer2();
        List<Analyzer> analyzers = new ArrayList<>();
        analyzers.add(analyzer1);
        analyzers.add(analyzer2);
        analyzers.add(analyzer3);
        
        
        for (File archivo : archivos) {
            if (archivo.isFile()) {
                // Crea el nombre del archivo CSV usando el nombre del archivo original
                String csvFileName = archivo.getName() + ".csv";

                // Crea un archivo CSV para escribir los resultados
                File csvFile = new File(csvFileName);        

                try (FileWriter fileWriter = new FileWriter(csvFile, StandardCharsets.UTF_8);
                     BufferedWriter writer = new BufferedWriter(fileWriter)) {
                    writer.write("Analizador 1; Fencuencia 1;Analizador 2; Fencuencia 2;Analizador 3;Fencuencia 3\n");

                    for (Analyzer analyzer : analyzers) {
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
                        TokenStream tokenStream = analyzer.tokenStream(null, text);
                        tokenStream.reset();

                        // Usar el tokenStream para recorrer los tokens del documento
                        CharTermAttribute charTermAttribute = tokenStream.addAttribute(CharTermAttribute.class);
                        Map<String, Integer> termFrequencyMap = new HashMap<>();

                        while (tokenStream.incrementToken()) {
                            String token = charTermAttribute.toString();
                            // Actualiza la frecuencia del término en el mapa
                            if (!token.isEmpty()) {
                                termFrequencyMap.put(token, termFrequencyMap.getOrDefault(token, 0) + 1);
                            }
                        }

                        // Ordena los términos por frecuencia en orden decreciente
                        List<Map.Entry<String, Integer>> sortedTermFrequencyList = termFrequencyMap.entrySet()
                                .stream()
                                .sorted((e1, e2) -> e2.getValue().compareTo(e1.getValue()))
                                .collect(Collectors.toList());
                        // Escribe los resultados en el archivo CSV
                        for (Map.Entry<String, Integer> entry : sortedTermFrequencyList) {
                            if (analyzer == analyzer1) {
                                // Resultados del analizador 1
                                writer.write(entry.getKey() + ";" + entry.getValue() + ";;" + "\n");
                            } else if (analyzer == analyzer2) {
                                // Resultados del analizador 2
                                writer.write(";;" + entry.getKey() + ";" + entry.getValue() + ";" + "\n");
                            } else if (analyzer == analyzer3) {
                                // Resultados del analizador 3
                                writer.write(";;;;" + entry.getKey() + ";" + entry.getValue() + "\n");
                            }
                        }
                    }

                    System.out.println("Se ha generado el archivo CSV con la frecuencia de términos: " + csvFileName);
                    
                } catch (IOException e) { e.printStackTrace(); }
            }
        }
    }
        // Analyzer an = new WhitespaceAnalyzer();
}