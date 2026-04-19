import java.util.Scanner;

public class UserInput {

   public static void main(String[] args) {
      // User input in most case scenarios
      try (Scanner myObj = new Scanner(System.in)) {
         System.out.print("Enter username: ");
         String userName = myObj.nextLine();

         System.out.println("My username is: " + userName);
      }
   }
}
