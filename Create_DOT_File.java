import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class CreateDOTFile {

	public static void aaa() {

		// make |E|
		int E = 100000;
		int V = 1800;
		String[] f1 = new String[E];
		for (int i = 0; i < f1.length; i++) {
			f1[i] = (float) Math.random() + "";
			if (f1[i].length() > 8) {
				f1[i] = f1[i].substring(0, 8);
			}
		}

		try {
			// Assume default encoding.
			FileWriter fileWriter = new FileWriter("temp.txt");

			// Always wrap FileWriter in BufferedWriter.
			BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);

			// Note that write() does not automatically
			// append a newline character.
			bufferedWriter.write("graph test1{");
			bufferedWriter.newLine();
			int end = 0;

			for (int i = 0; i < V; i++) {
				for (int j = i + 1; j < V; j++) {

					bufferedWriter.write((i + 1) + "--" + (j + 1) + "[weight="
							+ f1[(int) (Math.random() * 99999)] + "];");
					bufferedWriter.newLine();
				}
			}
			bufferedWriter.write("}");
			// Always close files.
			bufferedWriter.close();
		} catch (IOException ex) {
			System.out.println("Error writing to file 'temp.txt'");
			// Or we could just do this:
			// ex.printStackTrace();
		}
	}

	public static void main(String[] args) {
		aaa();
	}
}
