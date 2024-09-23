/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.mavenproject1;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import org.apache.tika.metadata.Metadata;
import org.apache.tika.parser.AutoDetectParser;
import org.apache.tika.parser.ParseContext;
import org.apache.tika.parser.Parser;
import org.apache.tika.sax.BodyContentHandler;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.io.Writer;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.stream.Collectors;
import org.apache.tika.exception.TikaException;
import org.apache.tika.langdetect.optimaize.OptimaizeLangDetector;
import org.apache.tika.language.detect.LanguageDetector;
import org.apache.tika.language.detect.LanguageResult;
import org.apache.tika.sax.Link;
import org.apache.tika.sax.LinkContentHandler;
import org.xml.sax.SAXException;

public class ExtractDocumentInfo {
    
    private static String detectarTipo(File archivo) throws IOException {
        Parser parser = new AutoDetectParser();
        Metadata metadata = new Metadata();

        try (InputStream stream = new FileInputStream(archivo)) {
            parser.parse(stream, new BodyContentHandler(-1), metadata, new ParseContext());
        } catch (SAXException | TikaException ex) {
            Logger.getLogger(ExtractDocumentInfo.class.getName()).log(Level.SEVERE, null, ex);  
        }

        return metadata.get(Metadata.CONTENT_TYPE);
    }

    private static String detectarCodificacion(File archivo) throws IOException {
        Parser parser = new AutoDetectParser();
        Metadata metadata = new Metadata();

        try (InputStream stream = new FileInputStream(archivo)) {
            parser.parse(stream, new BodyContentHandler(-1), metadata, new ParseContext());
        } catch (SAXException | TikaException ex) {
            Logger.getLogger(ExtractDocumentInfo.class.getName()).log(Level.SEVERE, null, ex);
        }

        return metadata.get(Metadata.CONTENT_ENCODING);
    }
    
    private static String readContentFromFile(File archivo) throws IOException {
        try (InputStream stream = new FileInputStream(archivo)) {
            byte[] data = new byte[(int) archivo.length()];
            stream.read(data);
            return new String(data, "UTF-8"); // Suponiendo que los archivos son UTF-8
        }
    }

   private static String detectarIdioma(File archivo, LanguageDetector languageDetector) throws IOException {
        String contenido = readContentFromFile(archivo);
            List<LanguageResult> languageResults = languageDetector.detectAll(contenido);
            if (!languageResults.isEmpty()) {
                LanguageResult topResult = languageResults.get(0);
                return topResult.getLanguage();
            }
        
        return "Desconocido";
    }
    
    
    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Uso: java ExtractDocumentInfo <directorio>");
            System.exit(1);
        }
    
        LanguageDetector languageDetector = new OptimaizeLangDetector().loadModels();
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

                try {
                    
                    if (args[1].equals("-d")) { //IMPRIME LOS METADATOS DE LOS DOCUMENTOS
                        System.out.println("| Nombre del archivo | Tipo | Codificación | Idioma |");
                        System.out.println("|--------------------|------|--------------|--------|");
                       
                        for (File archivo : archivos) {
                            if (archivo.isFile()) {
                                
                                String nombreArchivo = archivo.getName();
                                String tipo = detectarTipo(archivo);
                                String codificacion = detectarCodificacion(archivo);
                                String idioma = detectarIdioma(archivo,languageDetector);
                                
                                System.out.printf("| %-26s | %-34s | %-24s | %-6s |\n",
                                        nombreArchivo, tipo, codificacion, idioma);
                            }
                        }
                    }
                    if (args[1].equals("-l")) { //IMPRIME TODOS LOS ENLACES DE LOS DOCUMENTOS
                        for (File archivo : archivos) {
                            System.out.println("\n \n Buscamos enlaces en el archivo: " + archivo.getName() + "\n \n");
                            
                            try (InputStream input = new FileInputStream(archivo)) {
                                Metadata metadata = new Metadata();
                                LinkContentHandler handler = new LinkContentHandler();

                                // Utiliza el parser AutoDetectParser para detectar automáticamente el tipo de archivo
                                AutoDetectParser parser = new AutoDetectParser();
                                parser.parse(input, handler, metadata);

                                // Obtiene los enlaces del contenido analizado
                                List<Link> links = handler.getLinks();

                                // Imprime los enlaces encontrados
                                for (Link link : links) {
                                    System.out.println("Enlace encontrado: " + link);
                                }
                            } catch (SAXException | TikaException ex) {
                                Logger.getLogger(ExtractDocumentInfo.class.getName()).log(Level.SEVERE, null, ex);
                            }
                        }
                    }
                    if (args[1].equals("-t")) { //GENERA UN CSV ORDENANDO LAS PALABRAS DE LOS DOCUMENTOS SEGÚN SU OCURRENCIA
                        
                        for (File archivo : archivos) {
                                                 
                            try (InputStream input = new FileInputStream(archivo)) {
                                Metadata metadata = new Metadata();
                                BodyContentHandler handler = new BodyContentHandler(-1);

                                // Utiliza el parser AutoDetectParser para detectar automáticamente el tipo de archivo
                                AutoDetectParser parser = new AutoDetectParser();
                                    try {
                                        parser.parse(input, handler, metadata);
                                    } catch (SAXException | TikaException ex) {
                                        Logger.getLogger(ExtractDocumentInfo.class.getName()).log(Level.SEVERE, null, ex);
                                    }

                                // Obtén el contenido del documento como texto
                                String text = handler.toString();

                                // divide el texto en palabras si hay un espacio o más entre ellas
                                String[] words = text.split("\\s+");

                                // Crea un mapa para realizar un seguimiento de la frecuencia de cada término
                                Map<String, Integer> termFrequencyMap = new HashMap<>();

                                // Calcula la frecuencia de cada término
                                for (String word : words) {
                                    //Cambia las palabras a minúsculas
                                    word = word.replaceAll("[^\\p{L}áéíóúÁÉÍÓÚüÜñÑ]+", "").toLowerCase();

                                    // Actualiza la frecuencia del término en el mapa
                                    // Si word ya existe en el mapa, su frecuencia se incrementa en 1, de lo contrario, 
                                    // se agrega al mapa con una frecuencia inicial de 1.
                                    if(!word.isEmpty())
                                        termFrequencyMap.put(word, termFrequencyMap.getOrDefault(word, 0) + 1);
                                }

                                // Ordena los términos por frecuencia en orden decreciente
                                // Obtiene un conjunto de las entradas (pares clave-valor) del mapa 
                                List<Map.Entry<String, Integer>> sortedTermFrequencyList = termFrequencyMap.entrySet()
                                        //convierte el conjunto de entradas en un flujo (stream) de entradas
                                        .stream()
                                        //ordena las entradas del flujo en función de la frecuencia asociada a esas entradas en orden descendente.
                                        .sorted((e1, e2) -> e2.getValue().compareTo(e1.getValue()))
                                        // recopila las entradas ordenadas en una lista.
                                        .collect(Collectors.toList());

                                // Crea el nombre del archivo CSV usando el nombre del archivo original
                                String csvFileName = archivo.getName() + ".csv";

                                // Crea un archivo CSV para escribir los resultados
                                File csvFile = new File(csvFileName);
                                //FileWriter se utiliza para escribir caracteres en un archivo.
                                FileWriter fileWriter = new FileWriter(csvFile);
                                try (BufferedWriter writer = new BufferedWriter(fileWriter)) {
                                    // Escribe los términos y sus frecuencias en el archivo CSV
                                    for (Map.Entry<String, Integer> entry : sortedTermFrequencyList) {
                                        writer.write(entry.getKey() + ";" + entry.getValue() + "\n");
                                    }
                                    writer.close();
                                }
                                System.out.println("Se ha generado el archivo CSV con la frecuencia de términos: " + csvFileName);
                            }
                        }
                    }
                } catch (IOException e) {
                    System.err.println("Error al procesar el archivo.");
                }
            }
        }