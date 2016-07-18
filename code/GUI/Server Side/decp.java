
/*
Project name :  State Collecton GUI
Description  :  Server side state decryption code
Author's name:  Amanpreet Singh,Amit Raushan
Mentor's name:  Shubham Gupta
File name:      decp.java
*/
// imported libraries
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.Key;
import java.security.KeyPair;
import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.cert.Certificate;
import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author ansh-hp
 */
public class decp {
    
    /*
* Function Name: getkey()
* Input: none
* Output: KeyPair
* Description: Opens the keystore and reads the private key from it.
* Example Call: getkey();
     */
    
    public static KeyPair getkey() throws Exception {

        FileInputStream is = new FileInputStream("keyStoreName.jceks");         // Opens the KeyStore "keyStoreName.jceks" in read mode.

        KeyStore keystore = KeyStore.getInstance("JCEKS");                      // Initialises a KeyStore in "JCEKS" format.
        keystore.load(is, "EYSIP2016".toCharArray());                           // Loads the KeyStore.

        String alias = "Private Key";                                           // Stores "Private Key" in a string
        KeyPair keyPair = null;                                                 // Initialises a KeyPair to null

        Key key = keystore.getKey(alias, "EYSIP2016".toCharArray());            // Reads the Key from KeyStore.
        if (key instanceof PrivateKey) {                                        // Checks the key is Private Key
            // Get certificate of public key        
            Certificate cert = keystore.getCertificate(alias);                  // Reads the certifiacte from the KeyStore.
///////////
            // Get public key
            PublicKey publicKey = cert.getPublicKey();                          // Gets the Public Key using the certificate.

            // Return a key pair
            keyPair = new KeyPair(publicKey, (PrivateKey) key);                 // KeyPair is created using the private key and public key
        }

        return keyPair;                                                         // Returns the KeyPair.
    }

    public static void main(String args[]) throws IOException, Exception {
        // InetAddress ip = InetAddress.getLocalHost();

        KeyPair kp = getkey();                                                  // Gets the KeyPair.

        ServerSocket s = new ServerSocket(2194);                                // Creates the Server at socket no.2194

        System.out.println("Started: " + s);                                    // Displays on the console.
        String usrnm;                                                           // String to contain the username.

        while (true) {                                                          // Loop till user enters the correct login credentials.
           
            Socket s1 = s.accept();                                             // Accepts the client.
            DataInputStream dIn = new DataInputStream(s1.getInputStream());     // Variable to read the incoming data from the client.
            int length = dIn.readInt();                                         // Reads the integer coming from the client.
            byte[] usr = new byte[length];                                      // Creats a byte array of the length received.
            if (length > 0) {
                dIn.readFully(usr, 0, usr.length);                              // Reads the complete data from the client.
            }

            usrnm = new String(usr);                                            // Converts the byte array to string.

            System.out.println("USER NAME " + usrnm);                           // Displays the received username on console.

            dIn = new DataInputStream(s1.getInputStream());                     // Variable to read the incoming data from the client.
            length = dIn.readInt();                                             // Reads the integer coming from the client.
            byte[] pass = new byte[length];                                     // Creats a byte array of the length received.
            if (length > 0) {
                dIn.readFully(pass, 0, pass.length);                            // Reads the complete data from the client.
            }

            String pwd = new String(pass);                                      // Converts the byte array to string.
            System.out.println("Pass " + pwd);                                  // Displays the received username on console.

            byte b[];                                                           // Creates a buffer of type byte array.

            DataOutputStream dOut = new DataOutputStream(s1.getOutputStream()); // Variable to write data to the client.

            if (usrnm.equals("test") && pwd.equals("123")) {                    // Check if user has entered the correct login credentials.

                b = "accept".getBytes();                                        // Store "accept" in buffer
                dOut.writeInt(b.length);                                        // Write the length of the buffer to the client.
                dOut.write(b);                                                  // Write the data of the buffer to the client.
                System.out.println("User Accepted");                            // Display on console that user has been accepted.
                break;                                                          // Break out of the loop.
                

            } else {                                                            // If user enters invalid login credentials.

                b = "bad".getBytes();                                           // Store "bad" in buffer.
                dOut.writeInt(b.length);                                        // Write the length of the buffer to the client.
                dOut.write(b);                                                  // Write the data of the buffer to the client.
                System.out.println("Access Denied");                            // Display on console that user has been denied access.

            }
            s1.close();                                                         // Close the connection with the client.
        }
        
        Socket s4 = s.accept();                                                 // Accpet the connection to the client.
        
        DataOutputStream dOut = new DataOutputStream(s4.getOutputStream());     // Variable to write data to the client.
        byte[] b = usrnm.getBytes();                                            // Converting the String username to byte array.
        dOut.writeInt(b.length);                                                // Write the length of the buffer to the client.
        dOut.write(b);                                                          // Write the data of the buffer to the client.
        
        s4.close();                                                             // Close the connection with the client.

        Socket s3 = s.accept();                                                 // Accpet the connection to the client.

        DataInputStream dIn = new DataInputStream(s3.getInputStream());         // Variable to read the incoming data from the client.

        int length = dIn.readInt();                                             // Write the length of the buffer to the client.
        byte[] text = new byte[length];                                         // Write the data of the buffer to the client.
        if (length > 0) {
            dIn.readFully(text, 0, text.length);                                // Reads the complete data from the client.
        }

        String a = new String(text, "UTF-8");                                   // Converts the byte array to string and stores it in a variable 'a'. This contains the MD5 checksum of the original data.                                       

        s3.close();                                                             // Close the connection with the client.
        
        Socket s2 = s.accept();                                                 // Accpet the connection to the client.

        dIn = new DataInputStream(s2.getInputStream());                         // Variable to read the incoming data from the client.
        length = dIn.readInt();                                                 // Reads the integer coming from the client.
        byte[] m = new byte[length];                                            // Creats a byte array of the length received.
        if (length > 0) {
            dIn.readFully(m, 0, m.length);                                      // Reads the complete data from the client.
        }

        FileOutputStream fos = new FileOutputStream("AESKey.txt");              // Opens the File "AESKey.txt" in write mode.
        fos.write(m);                                                           // Writes the conetnts of variable "m" in the file.
        fos.close();                                                            // Closes the file.
        
        length = dIn.readInt();                                                 // Reads the integer coming from the client.
        m = new byte[length];                                                   // Creats a byte array of the length received.
        if (length > 0) {
            dIn.readFully(m, 0, m.length);                                      // Reads the complete data from the client.
        }
        fos = new FileOutputStream("output.txt");                               // Opens the File "output.txt" in write mode.
        fos.write(m);                                                           // Writes the conetnts of variable "m" in the file.
        fos.close();                                                            // Closes the file.

        String md5_2 = org.apache.commons.codec.digest.DigestUtils.md5Hex(m);   // Generates the MD5 Checksum of the received bytes.                          

        if (a.equals(md5_2)) {                                                  // If both the Checksums are same

            RandomAccessFile f = new RandomAccessFile("AESKey.txt", "r");       // Opens the File "AESKey.txt" in read mode.
            b = new byte[(int) f.length()];                                     // Finding the length of the file.
            f.read(b);                                                          // Reading the contents of the file.
            f.close();                                                          // Closes the file.
            
            Cipher desCipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");      // Creates a Cipher type object and initialises it for RSA type encryption/decryption with PKCS1Padding.
            desCipher.init(Cipher.DECRYPT_MODE, kp.getPrivate());               // Initialises Cipher in Decryption mode.
            byte[] textDecrypted = desCipher.doFinal(b);                        // Decrypts the data in b and stores it in textDecrypted.
            Key myDesKey = new SecretKeySpec(textDecrypted, 0, textDecrypted.length, "AES");        //Re-creates the AES key from the decrypted text.

            desCipher = Cipher.getInstance("AES/ECB/PKCS5Padding");             // Initialises the cipher for AES type encryption/decryption with PKCS5Padding.

            f = new RandomAccessFile("output.txt", "r");                        // Opens the File "output.txt" in read mode.
            System.out.println(f.length());                                     // Finding the length of the file.
            b = new byte[(int) f.length()];                                     // Reading the contents of the file.
            f.read(b);                                                          // Closes the file.

            desCipher.init(Cipher.DECRYPT_MODE, myDesKey);                      // Initialises Cipher in Decryption mode.
            textDecrypted = desCipher.doFinal(b);                               // Decrypts the data in b and stores it in textDecrypted.
            
            fos = new FileOutputStream("decp.txt");                             // Opens the File "decp.txt" in write mode.
            fos.write(textDecrypted);                                           // Writes the data textDecrypted to the file.
            fos.close();                                                        // Closes the file.
            
            System.out.println("No Mods");                                      // Displays "No Mods" on console.

        } else {
            System.out.println("Mods");                                         // Displays "Mods" on console.
        }

        s2.close();                                                             // Close the connection with the client.
        s.close();                                                              // Closes the server.
    }

}
