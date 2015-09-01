import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.StringTokenizer;

public class ConvertCsv2DotFile {
	static double[][] mat = new double[1728][1728];

	public static void aaa() {
		FileReader fr = null;

		try {
			fr = new FileReader("test1.csv");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		BufferedReader is = new BufferedReader(fr);
		try {
			for (int i = 0; i < mat[0].length; i++) {

				String s = is.readLine();
				StringTokenizer st = new StringTokenizer(s, ", ");
				int j = 0;
				while (st.hasMoreTokens()) {
					mat[i][j++] = Double.parseDouble(st.nextToken());
				}
				System.out.println();
			}
		} catch (Exception r) {
			// TODO: handle exception

		}

		// The name of the file to open.
		String fileName = "temp.txt";

		try {
			// Assume default encoding.
			FileWriter fileWriter = new FileWriter("temp.dot");

			// Always wrap FileWriter in BufferedWriter.
			BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);

			// Note that write() does not automatically
			// append a newline character.
			bufferedWriter.write("graph test1{");
			bufferedWriter.newLine();

			for (int i = 0; i < mat.length; i++) {
				for (int j = i + 1; j < mat.length; j++) {

					bufferedWriter.write((i + 1) + "--" + (j + 1) + "[weight="
							+ mat[i][j] + "];");
					bufferedWriter.newLine();
				}
			}
			bufferedWriter.write("}");
			// Always close files.
			bufferedWriter.close();
		} catch (IOException ex) {
			System.out.println("Error writing to file '" + fileName + "'");
			// Or we could just do this:
			// ex.printStackTrace();
		}
	}

	public static void main(String[] args) {
		aaa();
	}
}
