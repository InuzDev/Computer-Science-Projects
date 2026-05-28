import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;

public class DynamicArray {

   public static void main(String[] args) {
      // Create a list of strings using ArrayList. This is a dynamic array
      List<String> list = new ArrayList<>();

      // Fill the array
      list.add("Java");
      list.add("Python");
      list.add("C++");

      // Printing the header of the program:
      System.out.println("Programming languages object-oriented: ");
      for (String lang : list) {
         System.out.println(lang);
      }

      // I'm going to try by myself creating a dynamic array that involve the user input.
      System.out.println("Enter another programming language");
      String ExtraLanguage;

      try (Scanner GetUserInput = new Scanner(System.in)) {
         ExtraLanguage = GetUserInput.nextLine();
      }

      list.add(ExtraLanguage);

      System.out.println("Programming languages:");
      int index = 0;
      for (String lang : list) {
         System.out.println(
            "Programming Language: " + (index + 1) + " " + lang
         );
         index += 1;
      }
   }
}
