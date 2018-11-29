

import junit.framework.TestCase;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!





public class UrlValidatorTest extends TestCase {


   public UrlValidatorTest(String testName) {
      super(testName);
   }



   // Manual Test with default constructor
   public void testManualTest()
   {
	   UrlValidator urlValidator = new UrlValidator();
	   
	   // By default, http, https, and ftp are valid schemes
	   System.out.println("------ Manual Test with Default Constructor ------");
	   
	   // Valid URLs
	   System.out.println(" -VALID URLS-");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/"), true, "http://www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("https://www.google.com/"), true, "https://www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("ftp://www.google.com/"), true, "ftp://www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://google.com"), true, "http://google.com");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path"), true, "http://www.google.com/path");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path/"), true, "http://www.google.com/path/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path/file"), true, "http://www.google.com/path/file");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path?query"), true, "http://www.google.com/path?query");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path?query#fragment"), true, "http://www.google.com/path?query#fragment");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com:80"), true, "http://www.google.com:80");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com:65535"), true, "http://www.google.com:65535");
	   
	   // Invalid URLs
	   System.out.println(" -INVALID URLS-");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("www.google.com"), false, "www.google.com");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("3ht://www.google.com/"), false, "3ht://www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http:/www.google.com/"), false, "http:/www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("tp://www.google.com/"), false, "tp://www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://256.256.256.256/"), false, "http://256.256.256.256/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://.1.2.3.4/"), false, "http://.1.2.3.4/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/../"), false, "http://www.google.com/../");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path//file"), false, "http://www.google.com/path//file");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/..//path"), false, "http://www.google.com/..//path");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com:65536"), false, "http://www.google.com:65536");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com:-1"), false, "http://www.google.com:-1");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com:port"), false, "http://www.google.com:port");

	   System.out.println();
   }
 
   // Partition Test with Schemes Constructor
   public void testPartition1()
   {
	   String[] schemes = {"http","https"};
	   UrlValidator urlValidator = new UrlValidator(schemes);
	   System.out.println("------ Partition Test with Schemes Constructor ------");
	   
	   // http & https schemes should return true, ftp should return false
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/"), true, "http://www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("https://www.google.com/"), true, "https://www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("ftp://www.google.com/"), false, "ftp://www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("3ht://www.google.com/"), false, "3ht://www.google.com/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("tp://www.google.com/"), false, "tp://www.google.com/");
	   System.out.println();
   }
   
   
   // Partition Test with ALLOW_ALL_SCHEMES on
   public void testPartition2()
   {
	   UrlValidator urlValidator = new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES);
	   System.out.println("------ Partition Test with ALLOW_ALL_SCHEMES on ------");
	   
	   // All website should return true 
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/"), true, "http://www.google.com/");
	   try {
		   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("https://www.google.com/"), true, "https://www.google.com/");
	   } catch (Error e) {
		   System.out.println("*https://www.google.com/ is causing " + e);
	   }
	   try {
		   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("ftp://www.google.com/"), true, "ftp://www.google.com/");
	   } catch (Error e) {
		   System.out.println("*ftp://www.google.com/ is causing " + e);
	   }
	   try {
		   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("tp://www.google.com/"), true, "tp://www.google.com/");
	   } catch (Error e) {
		   System.out.println("*tp://www.google.com/ is causing " + e);
	   }
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("3ht://www.google.com/"), false, "3ht://www.google.com/");
	   
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("www.google.com/"), false, "www.google.com/");

	   // Test Authority
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://256.256.256.256/"), false, "http://256.256.256.256/");

	   // Test Path
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path"), true, "http://www.google.com/path");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path/"), true, "http://www.google.com/path/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/.."), false, "http://www.google.com/..");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/../"), false, "http://www.google.com/../");

	   // Test Query
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path?query"), true, "http://www.google.com/path?query");

	   System.out.println();

   }

   
   // Partition Test with ALLOW_ALL_SCHEMES and NO_FRAGMENTS on
   // Need to include ALLOW_ALL_SCHEMES, otherwise all web site would just return false
   public void testPartition3()
   {
	   UrlValidator urlValidator = new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES + UrlValidator.NO_FRAGMENTS);
	   System.out.println("------ Partition Test with NO_FRAGMENTS on ------");
	   
	   // URL with # should return false
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path#fragment"), false, "http://www.google.com/path#fragment");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/pathfragment"), true, "http://www.google.com/pathfragment");
	   System.out.println(); 
   }
   
   
   // Partition Test with ALLOW_ALL_SCHEMES and ALLOW_2_SLASHES on
   public void testPartition4()
   {
	   UrlValidator urlValidator = new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES + UrlValidator.ALLOW_2_SLASHES);
	   System.out.println("------ Partition Test with ALLOW_2_SLASHES on ------");
	   
	   // URL with / & // should return true
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path/Query"), true, "http://www.google.com/path/Query");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path//Query"), true, "http://www.google.com/path//Query");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://www.google.com/path///Query"), true, "http://www.google.com/path///Query");

	   System.out.println();	   
   }
   
   
   // Partition Test with ALLOW_ALL_SCHEMES and ALLOW_LOCAL_URLS on
   public void testPartition5()
   {
	   UrlValidator urlValidator = new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES + UrlValidator.ALLOW_LOCAL_URLS);
	   System.out.println("------ Partition Test with ALLOW_LOCAL_URLS on ------");
	   
	   // Should return true
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://localhost/"), true, "http://localhost/");
	   System.out.printf(" R:%5b E:%5b [%s]\n", urlValidator.isValid("http://machine/"), true, "http://machine/");
	   System.out.println();   
   }

   
     
   public void testIsValid()
   {
	   //You can use this function for programming based testing
	   
	   //Declare arrays of url pieces
	   //Valid pieces come first, then invalid
	   //scheme array
	   String[] schemes = new String[6];
	   schemes[0] = "http://";
	   schemes[1] = "https://";
	   schemes[2] = "ftp://";
	   schemes[3] = "";
	   schemes[4] = "tp://";
	   schemes[5] = "3ht://";
	   //authority array
	   String[] authorities = new String[5];
	   authorities[0] = "www.google.com";
	   authorities[1] = "google.com";
	   authorities[2] = ".1.2.3.4";
	   authorities[3] = "";
	   authorities[4] = "256.256.256.256";
	   //path array
	   String[] paths = new String[5];
	   paths[0] = "";
	   paths[1] = "/path/";
	   paths[2] = "/path";
	   paths[3] = "..";
	   paths[4] = "/../";
	   //query array
	   String[] queries = new String[2];
	   queries[0] = "";
	   queries[1] = "?query";
	   //fragment array
	   String[] fragments = new String[2];
	   fragments[0] = "";
	   fragments[1] = "#fragment";

	   
	   //variables to keep track of progress through the arrays
	   int schemeCount = 0;
	   int authCount = 0;
	   int pathCount = 0;
	   int queryCount = 0;
	   int fragmentCount = 0;
	   
	   //variables to keep track of the size of the arrays
	   int schemeSize = 6;
	   int authSize = 5;
	   int pathSize = 5;
	   int querySize = 2;
	   int fragmentSize = 2;
	   
	   //variables to track whether the array object is valid or not
	   int validSchemeLimit = 2;
	   int validAuthLimit = 1;
	   int validPathLimit = 2;
	   int validQueryLimit = 1;
	   int validFragmentLimit = 1;
	   
	   String done = "false";
	   
	   System.out.println("------ Unit Test with default settings ------");
	   
	   while (done == "false") {
		   //start building our url
		   
		   //scheme
		   String testMe = schemes[schemeCount];
		   Boolean expected = true;
		   if (schemeCount > validSchemeLimit)
			   expected = false;
		   
		   //authority
		   testMe = testMe + authorities[authCount];
		   if (authCount > validAuthLimit && authorities[authCount] != "http")
			   expected = false;

		   //path
		   testMe = testMe + paths[pathCount];
		   if (pathCount > validPathLimit)
			   expected = false;
				   
		   //query
		   testMe = testMe + queries[queryCount];
		   if (queryCount > validQueryLimit)
			   expected = false;

		   //fragment
		   testMe = testMe + fragments[fragmentCount];
		   if (fragmentCount > validFragmentLimit)
			   expected = false;
						   
		   //output test results
		   UrlValidator uv1 = new UrlValidator();
		   //System.out.println("------ Partition Test with default settings ------");
		   //System.out.println("Testing: " + testMe);
		   //System.out.println("Result: " + uv1.isValid(testMe) + " Expected: " + expected);
		   		   
		   if (uv1.isValid(testMe) != expected)
		   {
			   System.out.printf("*R:%5b E:%5b [%s]\n", uv1.isValid(testMe), expected, testMe);		 
		   }
		   else
		   {
			   System.out.printf(" R:%5b E:%5b [%s]\n", uv1.isValid(testMe), expected, testMe);

		   }
		   
		   schemeCount++;
		   if (schemeCount >= schemeSize) {
			   schemeCount = 0;
			   authCount++;
			   if (authCount >= authSize) {
				   authCount = 0;
				   pathCount ++;
				   if (pathCount >= pathSize) {
					   pathCount = 0;
					   queryCount++;
					   if (queryCount >= querySize) {
						   queryCount = 0;
						   fragmentCount++;
						   if (fragmentCount >= fragmentSize) {
							   done = "true";
						   }
					   }
				   }
			   }
		   }

	   }

   }



}