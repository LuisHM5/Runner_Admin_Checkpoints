using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using Runner_Admin_Checkpoints.Models;

namespace Runner_Admin_Checkpoints.Data
{
    public class Runner_Admin_CheckpointsContext : DbContext
    {
        public Runner_Admin_CheckpointsContext (DbContextOptions<Runner_Admin_CheckpointsContext> options)
            : base(options)
        {
        }

        public DbSet<Runner_Admin_Checkpoints.Models.RegistroViewModel> RegistroViewModel { get; set; } = default!;
    }
}
