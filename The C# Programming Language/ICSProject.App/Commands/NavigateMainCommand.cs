using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICSProject.App.Stores;

namespace ICSProject.App.Commands
{
    public class NavigateMainCommand : CommandBase
    {
        private readonly NavigationStore _navigationStore;

        public NavigateMainCommand(NavigationStore navigationStore)
        {
            _navigationStore = navigationStore;
        }
        public override void Execute(object parameter)
        {
            _navigationStore.CurrentViewModel = _navigationStore.MainViewModel;
        }
    }
}
