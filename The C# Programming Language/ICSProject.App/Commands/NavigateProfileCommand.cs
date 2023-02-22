using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICSProject.App.Stores;
using ICSProject.App.ViewModels;

namespace ICSProject.App.Commands
{
    public class NavigateProfileCommand : CommandBase
    {
        private readonly NavigationStore _navigationStore;

        public NavigateProfileCommand(NavigationStore navigationStore)
        {
            _navigationStore = navigationStore;

        }
        public override void Execute(object parameter)  
        {
            _navigationStore.CurrentViewModel = _navigationStore.ProfileViewModel;
        }
    }
}
