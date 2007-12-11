<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: YOUR_NAME
  // Creation date: someday/somemonth/someyear
  // RCS-ID: $Id: index.php,v 1.7 2005/09/01 22:26:16 frm Exp $
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php
      $compname = "wxLatexDC";
      $subdir = "latexdc";
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
	<br/><?php write_h1("Example"); ?>
	<p>Integrate drawings into LaTeX documents</p>
  <div class="center">
    <?php writeScreenshotLink("example.png", "A wxDC drawing integrated into a document with wxLatexDC"); ?>
  </div>



  <!-- NOTE ON THE SECTIONS TO INSERT HERE: you are free to add anything you think
       that will make your component more appealing: some sample snippet of code,
       docs, bugs & feature requests... -->
  <br/><?php write_h1("Usage sample"); ?>
  <?php writeSnippet('
// Usage of wxLatexDC is similar to other DCs (such as wxSVGFileDC):
// ...
wxLatexDC latexDC(filename,GetRect().width,GetRect().height,72.0);
Draw(latexDC);
// ...


% You can then include the output in a LaTeX document
% In your header, you need to include PSTricks:
\usepackage{pstricks}

% In your document, you simply integrate the tex file:
\input{./filename}
'); ?>


  <!-- NOTE TO THE DOCUMENTATION: for uploading you component docs in the website,
       see the ReadMe.txt file at wxCode\template... -->
  <br/><?php write_h1("Documentation"); ?>
  <p>The documentation for this component is available online
  <a href="../../docs/latexdc/docs/index.html">here</a>.</p>


	<br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../support.php">wxCode support page</a>.</p>

    
	<br/><?php write_h1("Future enhancements"); ?>
  <p>None. To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p>
    

	<br/><?php require("footer.inc.php"); ?>
</html>
