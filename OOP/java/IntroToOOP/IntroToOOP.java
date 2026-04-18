import java.util.ArrayList;

// A thing about Java, the class should carry the file's name.
public class IntroToOOP {

   // Let's define some properties of the class.
   Integer x = 5; // This is a method to define an integer
   int z = 5; // This is another method, I would call it a classic method.

   public static void main(String[] args) {
      /* Lets create an object */
      IntroToOOP myObj = new IntroToOOP();
      // You can create multiple objects.
      IntroToOOP SecondObject = new IntroToOOP();

      SecondObject.x = 5;
      SecondObject.z = 23;

      // Let's print out the object property.
      System.out.println(myObj.z + " " + myObj.x);
      System.out.println(SecondObject.x + " " + SecondObject.z);

      ArrayList<String> cars = new ArrayList<String>();

      cars.add("Volvo");
      cars.add("BMW");
      cars.add("Ford");
      cars.add("Mazda");

      System.out.println(cars);
   }
}
