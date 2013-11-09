var pageStack = [];

function push(page)
{
  if (currentPage)
    currentPage.visible = false;
  
  page.visible = true;
  pageStack.push(page);
  currentPage = page;
  return page;
}


function pop()
{
  if (currentPage)
    currentPage.visible = false;
  
  pageStack.pop();
  currentPage = pageStack.length > 0 ?
    pageStack[pageStack.length - 1] : homePage;

  currentPage.visible = true;
  return currentPage;
}


function clear()
{
  var page;
  while (page = pageStack.pop())
    page.visible = false;
  
  currentPage = homePage;
  currentPage.visible = true;
  return currentPage;
}
