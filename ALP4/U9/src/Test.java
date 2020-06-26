package assignments;

import java.io.File;
import java.io.IOException;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine.Info;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.UnsupportedAudioFileException;

public class Test {
	protected static String path = System.getProperty("user.dir") + "/assignments/resource.wav";
	
	public Test(String file) {
	    try {
			// read wav file to audio stream
			AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(new File(file));
			
			// read audio format from stream
			AudioFormat audioFormat = audioInputStream.getFormat();
			
			System.out.println("Sample rate: " + audioFormat.getSampleRate());
			System.out.println("Frames length: " + audioInputStream.getFrameLength());
			System.out.println("Time:" + audioInputStream.getFrameLength() / audioFormat.getSampleRate());
			
			// SourceDataLine info
			Info dataLineInfo = new Info(SourceDataLine.class, audioFormat);
			
			SourceDataLine sourceDataLine = (SourceDataLine) AudioSystem.getLine(dataLineInfo);
			byte[] b = new byte[102400];
			int len = 0;    
			sourceDataLine.open(audioFormat, 102400);
			sourceDataLine.start();
			while ((len = audioInputStream.read(b)) > 0) {
			    sourceDataLine.write(b, 0, len);
			}
			
			audioInputStream.close();
			sourceDataLine.drain();
			sourceDataLine.close();
	    } catch (UnsupportedAudioFileException e) {
			e.printStackTrace();
	    } catch (IOException e) {
	    	e.printStackTrace();
	    } catch (LineUnavailableException e) {
	    	e.printStackTrace();
	    }
    }

	public static void main(String[] args) {
        //System.out.println(System.getProperty("user.dir"));
	}
}

