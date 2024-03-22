// Controllers/HomeController.cs
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using YourNamespace.Data;
using YourNamespace.Models;

[Authorize]
public class HomeController : Controller
{
    private readonly ApplicationDbContext _context;

    public HomeController(ApplicationDbContext context)
    {
        _context = context;
    }

    public IActionResult Index()
    {
        var contact = _context.Contacts.Find(User.Identity.Name); // Use user name as ID for simplicity
        return View(contact);
    }

    [HttpPost]
    public IActionResult Index(Contact contact)
    {
        if (ModelState.IsValid)
        {
            _context.Contacts.Update(contact);
            _context.SaveChanges();
            return RedirectToAction("Index");
        }
        return View(contact);
    }
}
