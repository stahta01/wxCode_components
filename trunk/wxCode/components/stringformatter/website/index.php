<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Nathan Shaffer
  // Creation date: 30/january/2013
  // RCS-ID: $Id: index.php 505 2007-03-31 10:31:46Z frm $
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php
      $compname = "wxStringFormatter";
      $subdir = "wxStringFormatter";
      require("compwebsite.inc.php");
  ?>

	<!-- NOTE on the write_h1() php function:
         - first arg: the label of the h1 tag
         - second arg: the ID of the h1 tag (defaults to "")
         - third arg: if TRUE the TOP link is added; if FALSE it is omitted (defaults to TRUE)
         use FALSE as third arg for the first h1 tag of the page since it must
         not have the TOP link...
  -->


	<!-- NOTE on SCREENSHOTS:
       to write screenshots link with automatic thumbnail-generation, you can write something like:

       <?php writeScreenshotLink("myshot.png", "MYCOMP running with wxMSW 2.5.3"); ?>

	     where the 'myshot.png' file is a file that you have uploaded in
          /home/groups/w/wx/wxcode/htdocs/screenshots/MYCOMPSUBDIR
       you can do the upload with scp from unix or (I suggest), WinSCP (http://winscp.net)
       from win32... more info in the wxCode\template\readme.txt file.

       To put something centered in this page, put it inside a

          <div class="center">...</div>

       block...
  -->
	<br/><?php write_h1("Screenshots"); ?>
	<p>Description</p>
  <div class="center">
    <?php writeScreenshotLink("image.png", ""); ?>
  </div>



  <!-- NOTE ON THE SECTIONS TO INSERT HERE: you are free to add anything you think
       that will make your component more appealing: some sample snippet of code,
       docs, bugs & feature requests... -->
  <br/><?php write_h1("Usage sample"); ?>
  <?php writeSnippet('
//wxFormatFunction create new inherited classes of wxFormatFunction to provide custom functionality. ...
//example:

	class expandNumberFunc : public wxFormatFunction
	{
	public:
		expandNumberFunc();
		~expandNumberFunc();
		wxString Parse(wxString input)
		{
			...
			change numerals to written numbers
			...
		}
	};

	wxStringFormatter SF;
	SF.AddFormatFunction("EXPAND", new expandNumberFunc());

	wxString sample = "I have EXPAND(1) apple."

	SF.Parse(sample);

	output = I have one apple.

'); ?>


  <!-- NOTE TO THE DOCUMENTATION: for uploading you component docs in the website,
       see the ReadMe.txt file at wxCode\template... -->
  <br/><?php write_h1("Documentation"); ?>
  <p>The documentation for this component is available online
  <a href="../../docs/wxstringformatter/">here</a>.</p>


	<br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../support.php">wxCode support page</a>.</p>

    
	<br/><?php write_h1("Future enhancements"); ?>
  <p>None. To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p>
    

	<br/><?php require("footer.inc.php"); ?>
</html>
