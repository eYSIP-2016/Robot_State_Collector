
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.Key;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.cert.Certificate;
import java.security.spec.X509EncodedKeySpec;
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

    public static KeyPair getkey() throws Exception {

        FileInputStream is = new FileInputStream("keyStoreName.jceks");

        KeyStore keystore = KeyStore.getInstance("JCEKS");
        keystore.load(is, "EYSIP2016".toCharArray());

        String alias = "Private Key";
        KeyPair keyPair = null;

        Key key = keystore.getKey(alias, "EYSIP2016".toCharArray());
        if (key instanceof PrivateKey) {
            // Get certificate of public key
            Certificate cert = keystore.getCertificate(alias);

            // Get public key
            PublicKey publicKey = cert.getPublicKey();

            // Return a key pair
            keyPair = new KeyPair(publicKey, (PrivateKey) key);
        }

        return keyPair;
    }

    public static void main(String args[]) throws IOException, Exception {
        // InetAddress ip = InetAddress.getLocalHost();
        ServerSocket s = new ServerSocket(2194);
        System.out.println("Started:" + s);
        Socket s1 = s.accept();
        DataInputStream dIn = new DataInputStream(s1.getInputStream());
        int length = dIn.readInt();
        byte[] sig = new byte[length];
        if (length > 0) {
            dIn.readFully(sig, 0, sig.length);
        }
        System.out.println("Signature bytes" + sig);

        dIn = new DataInputStream(s1.getInputStream());
        length = dIn.readInt();
        byte[] m = new byte[length];
        if (length > 0) {
            dIn.readFully(m, 0, m.length);
        }
        FileOutputStream fos = new FileOutputStream("AESKey.txt");
        fos.write(m);
        fos.close();
        length = dIn.readInt();
        m = new byte[length];
        if (length > 0) {
            dIn.readFully(m, 0, m.length);
        }
        fos = new FileOutputStream("output.txt");
        fos.write(m);
        fos.close();

        length = dIn.readInt();
        m = new byte[length];
        if (length > 0) {
            dIn.readFully(m, 0, m.length);
        }
        fos = new FileOutputStream("publicKeyFromUser.txt");
        fos.write(m);
        fos.close();
        
        Signature sign = Signature.getInstance("MD5WithRSA");
        

        RandomAccessFile f = new RandomAccessFile("publicKeyFromUser.txt", "r");
        byte[] b = new byte[(int) f.length()];
        f.read(b);
        PublicKey publicKey = KeyFactory.getInstance("RSA").generatePublic(new X509EncodedKeySpec(b));
        sign.initVerify(publicKey);
        

        KeyPair kp = getkey();
        f = new RandomAccessFile("AESKey.txt", "r");
        b = new byte[(int) f.length()];
        f.read(b);
        Cipher desCipher;
        desCipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        desCipher.init(Cipher.DECRYPT_MODE, kp.getPrivate());
        byte[] textDecrypted = desCipher.doFinal(b);
        Key myDesKey = new SecretKeySpec(textDecrypted, 0, textDecrypted.length, "AES");

        String stringKey = myDesKey.toString();
        System.out.println("read Key: " + stringKey);

        desCipher = Cipher.getInstance("AES/ECB/PKCS5Padding");

        f = new RandomAccessFile("output.txt", "r");
        System.out.println(f.length());
        b = new byte[(int) f.length()];
        f.read(b);
        
        sign.update(b);
        
        System.out.println(sign.verify(sig));
        
        System.out.println(b.length);
        desCipher.init(Cipher.DECRYPT_MODE, myDesKey);
        textDecrypted = desCipher.doFinal(b);
        fos = new FileOutputStream("decp.txt");
        fos.write(textDecrypted);
        fos.close();

        s1.close();
        s.close();
    }

}
