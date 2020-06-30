package grading;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import org.junit.Test;

/**
 * Add your own test cases group in methods. JUnit will execute all
 * parameterless public-void methods that are annotated with <b>@Test</b>.
 * <p>
 * Execute the test cases by right-clicking on <code>GraderTest.java</code> in
 * the Package Explorer, and choosing "Run As" > "JUnit Test".
 * <p>
 * <a href='http://junit.org/junit4/cookbook.html'>Learn more about JUnit</a>
 * 
 */
public class GraderTest {
	@Test
	public void nonsenseTest() {
		assertEquals("Math should work", 2, 1 + 1);
	}

	@Test
	public void graderAccessTest() {
		assertNotNull("Grader should be accessible", Grader.grade(0, 0));
	}
	
	@Test 
	public void testOfFD() {
		assertEquals(Grader.Grade.F, Grader.grade(19, 21));
		assertEquals(Grader.Grade.D, Grader.grade(20, 20));
	}
	
	@Test
	public void testOfCD() {
		assertEquals(Grader.Grade.D, Grader.grade(20, 20));
		assertEquals(Grader.Grade.C, Grader.grade(20, 40));
	}
	
	@Test
	public void testOfBC() {
		assertEquals(Grader.Grade.B, Grader.grade(30, 45));
		assertEquals(Grader.Grade.C, Grader.grade(29, 45));
	}
	
	@Test
	public void testOfAB() {
		assertEquals(Grader.Grade.B, Grader.grade(30, 46));
		assertEquals(Grader.Grade.A, Grader.grade(35, 55));
	}
	
	@Test
	public void testOfFC() {
		assertEquals(Grader.Grade.F, Grader.grade(19, 40));
		assertEquals(Grader.Grade.C, Grader.grade(20, 40));
	}

	@Test
	public void testOfFB() {
		assertEquals(Grader.Grade.F, Grader.grade(19, 55));
		assertEquals(Grader.Grade.B, Grader.grade(20, 55));
	}
	
	@Test
	public void testOfFA() {
		assertEquals(Grader.Grade.F, Grader.grade(19, 30));
		assertEquals(Grader.Grade.A, Grader.grade(30, 60));
	}
}
