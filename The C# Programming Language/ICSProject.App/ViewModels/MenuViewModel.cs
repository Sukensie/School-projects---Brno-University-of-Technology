using System;
using System.Collections.ObjectModel;
using System.Dynamic;
using System.Threading.Tasks;
using System.Windows.Input;
using ICSProject.App.Commands;
using ICSProject.BL.Facades;
using ICSProject.BL.Models;
using ICSProject.App.Extensions;
using ICSProject.App.Services;
using ICSProject.App.Messages;
using ICSProject.App.Stores;
using ICSProject.App.Wrappers;
using ICSProject.DAL.Entities;
using ICSProject.DAL.Seeds;

namespace ICSProject.App.ViewModels
{
    public class MenuViewModel : ViewModelBase, IMenuViewModel
    {
        private readonly UserFacade _userFacade;
        public MenuViewModel(NavigationStore navigationStore, UserFacade userFacade)
        {
            NavigateCreateUserCommand = new NavigateCreateUserCommand(navigationStore);
            NavigateRidesCommand = new NavigateRidesCommand(navigationStore);
            NavigateProfileCommand = new NavigateProfileCommand(navigationStore);
            NavigateMainCommand = new NavigateMainCommand(navigationStore);
            NavigateLogoutCommand = new NavigateLogoutCommand(navigationStore);
            NavigateGarageCommand = new NavigateGarageCommand(navigationStore);
            NavigateCreateRideCommand = new NavigateCreateRideCommand(navigationStore);

            _userFacade = userFacade;
        }

        public UserDetailModel UserDetail { get; set; }


        public ICommand NavigateRidesCommand { get; }
        public ICommand NavigateCreateUserCommand { get; }
        public ICommand NavigateProfileCommand { get; }
        public ICommand NavigateMainCommand { get; }
        public ICommand NavigateLogoutCommand { get; }
        public ICommand NavigateGarageCommand { get; }
        public ICommand NavigateCreateRideCommand { get; }
        //K tomu aby pri editaci uzivatele se aktualizovalo jmeno/email co je v menu
        public async Task LoadAsync()
        {
            var id = this.UserDetail.Id;
            var usr = await _userFacade.GetAsync(id);
            UserDetail = usr;
        }
    }
}