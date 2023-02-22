using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICSProject.App.Services;

namespace ICSProject.App.ViewModels
{
    public class LoginViewModel : ViewModelBase
    {

        private readonly IMediator _mediator;
        public LoginViewModel(
            UsersListViewModel usersListViewModel,
            IMediator mediator
        )
        {
            UsersListViewModel = usersListViewModel;
            _mediator = mediator;
        }

        public UsersListViewModel UsersListViewModel { get; }
    }
}
