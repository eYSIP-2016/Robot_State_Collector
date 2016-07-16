
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.security.InvalidKeyException;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.PrivateKey;
import java.security.Security;
import java.security.SignatureException;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.Calendar;
import java.util.Date;

//import org.bouncycastle.jce.provider.*;
import org.bouncycastle.jce.X509Principal;
import org.bouncycastle.x509.X509V3CertificateGenerator;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author Amanpreet Singh
 */
public class serial {

    /*
* Function Name: generateCertificate()
* Input: KeyPair
* Output: Certificate
* Description: Generates a certificate with a validity of 100000 Days.
* Example Call: generateCertificate(keyPair);
     */
    public static X509Certificate generateCertificate(KeyPair keyPair) throws CertificateEncodingException, IllegalStateException, NoSuchProviderException, NoSuchAlgorithmException, SignatureException, InvalidKeyException {
        X509V3CertificateGenerator cert = new X509V3CertificateGenerator();                 // Creats a certificate generator 
        Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());     // Adds a provider 
        cert.setSerialNumber(BigInteger.valueOf(1));                                        // Sets the serial no.of the certificate
        cert.setSubjectDN(new X509Principal("CN=localhost"));                               // Sets the Subject of the certificate
        cert.setIssuerDN(new X509Principal("CN=localhost"));                                // Sets the Issuer of the Certificate
        cert.setPublicKey(keyPair.getPublic());                                             // Sets the Public Key for the Certificate
        cert.setNotBefore(new Date(System.currentTimeMillis()));                            // Sets the Date of Validation of the Certificate
        Calendar cal = Calendar.getInstance();                                              // Initialises a Calender type object
        Date d = new Date();                                                                // Initialises a Date type Object
        cal.setTime(d);                                                                     // Sets the current date as calendar's time
        cal.add(Calendar.DATE, 100000);                                                     // Adds 100000 Days to the calendar's time
        d = cal.getTime();                                                                  // Sets D to 100000 Days ahead of current date.

        cert.setNotAfter(new Date(d.getTime()));                                            // Sets the Expiry Date of the Certificate
        cert.setSignatureAlgorithm("SHA1WithRSAEncryption");                                // Sets the Algorithm for Signing the Certificate.
        PrivateKey signingKey = keyPair.getPrivate();                                       // Sets the Key used to sign the Certificate.

        return cert.generate(signingKey, "BC");                                             // Returns the Signed certificate 
    }

    public static void main(String args[]) throws KeyStoreException, FileNotFoundException, IOException, NoSuchAlgorithmException, CertificateException, CertificateEncodingException, IllegalStateException, NoSuchProviderException, SignatureException, InvalidKeyException {

        KeyStore ks;                                                                        // Creates a KeyStore type object
        ks = KeyStore.getInstance("JCEKS");                                                 // Intiaises a KeyStore Type object as JCEKS as the keystore used by us is a JCEKS type of format. it can even be of JKS.
        String password = "EYSIP2016";                                                      // The password used to access the KeyStore
        char[] xy = password.toCharArray();                                                 // Change the format of password to char array.

        java.io.FileInputStream fis = null;                                                 // Creates a FileInputStream Type Object.
        java.io.FileOutputStream fos = null;                                                // Creates a FileOutputStream type Obejct.
        try {
            fis = new java.io.FileInputStream("keyStoreName.jceks");                        // Initialises fis to keyStoreName.jceks
            ks.load(fis, xy);                                                               // Loads the KeyStore
        } finally {
            if (fis != null) {
                fis.close();                                                                // Close fis.
            }
        }
        KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("RSA");                                // Creats a KeyPairGenerator used for asymmetric key encryption.  
        keyPairGenerator.initialize(1024);                                                                      // Initialises the KeyPairGenerator with a key size of 1024. 
        KeyPair keyPair = keyPairGenerator.genKeyPair();                                                        // Generates and stores the KeyPair.
        X509Certificate certificate = generateCertificate(keyPair);                                             // Generates and stores the Certificate.
        Certificate[] certChain = new Certificate[1];                                                           // Creates a Certificate array of size 1. Required for storing the key in the KeyStore.
        certChain[0] = certificate;                                                                             // Stores the Generated Certificate in the newly created array. 
        KeyStore.ProtectionParameter protParam = new KeyStore.PasswordProtection(xy);                           // Sets the Passwword used for accessing the KeyStore. 
        KeyStore.PrivateKeyEntry skEntry = new KeyStore.PrivateKeyEntry(keyPair.getPrivate(), certChain);       // Sets the Private Key to be stored in the KeyStore.
        ks.setEntry("Private Key", skEntry, protParam);                                                         // Makes Private key ready to be inserted into the KeyStore.  
        try {
            fos = new java.io.FileOutputStream("keyStoreName.jceks");                                           // Creates a FileOutputStream type Obejct and initialises it to "keyStoreName.jceks".
            ks.store(fos, xy);                                                                                  // Stores the Private Key in the Keystore.
            String stringKey = keyPair.getPrivate().toString();                                                 // Stores the Private Key as a String.
            System.out.println("Stored Key: " + stringKey);                                                     // Displays the Private Key stored. 
        } finally {
            if (fos != null) {
                fos.close();                                                                                    // Closes "keyStoreName.jceks"
            }
        }
        FileOutputStream fos2 = new FileOutputStream("publicKey.txt");                                          // Creates a FileOutputStream type Obejct and initialises it to "publicKey.txt".                                                  
        fos2.write(keyPair.getPublic().getEncoded());                                                           // Stores the byte array of the Public Key in "publicKey.txt". This txt file should be provided with the client side code.
        fos2.close();                                                                                           // Closes "publicKey.txt".
    }

}
