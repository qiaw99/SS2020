import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

public class Refl {
	public static void main(String[] args) throws NoSuchMethodException, SecurityException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException {
		Class<?> c = String.class;
		Constructor<?> constructor = c.getConstructor(String.class);
		Object object = constructor.newInstance("23313333");
		System.out.println(object);
	}
}
