using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using Runner_Admin_Checkpoints.Data;

namespace Runner_Admin_Checkpoints.Models
{
    public class HomeController : Controller
    {
        private readonly Runner_Admin_CheckpointsContext _context;

        public HomeController(Runner_Admin_CheckpointsContext context)
        {
            _context = context;
        }

        // GET: Home
        public async Task<IActionResult> Index()
        {
              return _context.RegistroViewModel != null ? 
                          View(await _context.RegistroViewModel.ToListAsync()) :
                          Problem("Entity set 'Runner_Admin_CheckpointsContext.RegistroViewModel'  is null.");
        }

        // GET: Home/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null || _context.RegistroViewModel == null)
            {
                return NotFound();
            }

            var registroViewModel = await _context.RegistroViewModel
                .FirstOrDefaultAsync(m => m.ID == id);
            if (registroViewModel == null)
            {
                return NotFound();
            }

            return View(registroViewModel);
        }

        // GET: Home/Create
        public IActionResult Create()
        {
            return View();
        }

        // POST: Home/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("ID,NombreTarjeta,Tiempo")] RegistroViewModel registroViewModel)
        {
            if (ModelState.IsValid)
            {
                _context.Add(registroViewModel);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(registroViewModel);
        }

        // GET: Home/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null || _context.RegistroViewModel == null)
            {
                return NotFound();
            }

            var registroViewModel = await _context.RegistroViewModel.FindAsync(id);
            if (registroViewModel == null)
            {
                return NotFound();
            }
            return View(registroViewModel);
        }

        // POST: Home/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("ID,NombreTarjeta,Tiempo")] RegistroViewModel registroViewModel)
        {
            if (id != registroViewModel.ID)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    _context.Update(registroViewModel);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!RegistroViewModelExists(registroViewModel.ID))
                    {
                        return NotFound();
                    }
                    else
                    {
                        throw;
                    }
                }
                return RedirectToAction(nameof(Index));
            }
            return View(registroViewModel);
        }

        // GET: Home/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null || _context.RegistroViewModel == null)
            {
                return NotFound();
            }

            var registroViewModel = await _context.RegistroViewModel
                .FirstOrDefaultAsync(m => m.ID == id);
            if (registroViewModel == null)
            {
                return NotFound();
            }

            return View(registroViewModel);
        }

        // POST: Home/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            if (_context.RegistroViewModel == null)
            {
                return Problem("Entity set 'Runner_Admin_CheckpointsContext.RegistroViewModel'  is null.");
            }
            var registroViewModel = await _context.RegistroViewModel.FindAsync(id);
            if (registroViewModel != null)
            {
                _context.RegistroViewModel.Remove(registroViewModel);
            }
            
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool RegistroViewModelExists(int id)
        {
          return (_context.RegistroViewModel?.Any(e => e.ID == id)).GetValueOrDefault();
        }

        public ActionResult Index()
        {
            var registros = _context.RegistrosEventos
                .Include(r => r.TarjetaRFID)
                .Select(r => new RegistroViewModel
                {
                    ID = r.ID,
                    NombreTarjeta = r.TarjetaRFID.NombreTarjeta,
                    Tiempo = r.FechaHora.ToString("HH:mm:ss")
                })
                .ToList();

            return View(registros);
        }
    }
}
