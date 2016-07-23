
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

    public static X509Certificate generateCertificate(KeyPair keyPair) throws CertificateEncodingException, IllegalStateException, NoSuchProviderException, NoSuchAlgorithmException, SignatureException, InvalidKeyException {
        X509V3CertificateGenerator cert = new X509V3CertificateGenerator();
        Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());
        cert.setSerialNumber(BigInteger.valueOf(1));   //or generate a random number  
        cert.setSubjectDN(new X509Principal("CN=localhost"));  //see examples to add O,OU etc  
        cert.setIssuerDN(new X509Principal("CN=localhost")); //same since it is self-signed  
        cert.setPublicKey(keyPair.getPublic());
        cert.setNotBefore(new Date(System.currentTimeMillis()));
        Calendar cal = Calendar.getInstance();
        Date d = new Date();
        cal.setTime(d);
        cal.add(Calendar.DATE, 100000);
        d = cal.getTime();

        cert.setNotAfter(new Date(d.getTime()));
        cert.setSignatureAlgorithm("SHA1WithRSAEncryption");
        PrivateKey signingKey = keyPair.getPrivate();
        return cert.generate(signingKey, "BC");
    }

    public static void main(String args[]) throws KeyStoreException, FileNotFoundException, IOException, NoSuchAlgorithmException, CertificateException, CertificateEncodingException, IllegalStateException, NoSuchProviderException, SignatureException, InvalidKeyException {
        KeyStore ks;
        ks = KeyStore.getInstance("JCEKS");
        String password = "EYSIP2016";
        char[] xy = password.toCharArray();
        java.io.FileInputStream fis = null;
        java.io.FileOutputStream fos = null;
        try {
            fis = new java.io.FileInputStream("keyStoreName.jceks");
            ks.load(fis, xy);
        } finally {
            if (fis != null) {
                fis.close();
            }
        }
        KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("RSA");
        keyPairGenerator.initialize(1024);
        KeyPair keyPair = keyPairGenerator.genKeyPair();
        X509Certificate certificate = generateCertificate(keyPair);
        Certificate[] certChain = new Certificate[1];
        certChain[0] = certificate;
        KeyStore.ProtectionParameter protParam = new KeyStore.PasswordProtection(xy);
        KeyStore.PrivateKeyEntry skEntry = new KeyStore.PrivateKeyEntry(keyPair.getPrivate(), certChain);
        ks.setEntry("Private Key", skEntry, protParam);
        try {
            fos = new java.io.FileOutputStream("keyStoreName.jceks");
            ks.store(fos, xy);
            String stringKey = keyPair.getPrivate().toString();
            System.out.println("Stored Key: " + stringKey);
        } finally {
            if (fos != null) {
                fos.close();
            }
        }
        FileOutputStream fos2 = new FileOutputStream("publicKey.txt");
            fos2.write(keyPair.getPublic().getEncoded());
            fos2.close();
    }
        
}
